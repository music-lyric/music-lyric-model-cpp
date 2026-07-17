package main

import (
	"flag"
	"fmt"
	"io"
	"os"
	"path"
	"strings"

	"google.golang.org/protobuf/compiler/protogen"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/types/descriptorpb"
	"google.golang.org/protobuf/types/pluginpb"
)

type config struct {
	moduleNS string
	emit     string // "all", "domain", or "bridge"
}

var cfg = config{
	moduleNS: "music_lyric_model",
	emit:     "all",
}

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}

func run() error {
	in, err := io.ReadAll(os.Stdin)
	if err != nil {
		return err
	}
	req := &pluginpb.CodeGeneratorRequest{}
	if err := proto.Unmarshal(in, req); err != nil {
		return err
	}

	for _, f := range req.ProtoFile {
		injectGoPackage(f)
	}

	rootNS := "music_lyric_model"
	emit := "all"
	var kept []string
	for _, part := range strings.Split(req.GetParameter(), ",") {
		if part == "" {
			continue
		}
		kv := strings.SplitN(part, "=", 2)
		key := kv[0]
		val := ""
		if len(kv) == 2 {
			val = kv[1]
		}
		switch key {
		case "root_ns", "cpp_ns":
			if val != "" {
				rootNS = val
			}
		case "module":
			// Reserved by protogen; reuse value as root_ns only.
			if val != "" {
				rootNS = val
			}
		case "emit":
			if val != "" {
				emit = val
			}
		default:
			kept = append(kept, part)
		}
	}
	cfg.moduleNS = rootNS
	cfg.emit = emit
	req.Parameter = proto.String(strings.Join(kept, ","))

	var flags flag.FlagSet
	opts := protogen.Options{ParamFunc: flags.Set}
	gen, err := opts.New(req)
	if err != nil {
		return err
	}
	gen.SupportedFeatures = uint64(pluginpb.CodeGeneratorResponse_FEATURE_PROTO3_OPTIONAL)

	for _, file := range gen.Files {
		if !file.Generate {
			continue
		}
		if cfg.emit == "all" || cfg.emit == "domain" {
			if err := generateDomainFile(gen, file); err != nil {
				return fmt.Errorf("%s domain: %w", file.Desc.Path(), err)
			}
		}
		if cfg.emit == "all" || cfg.emit == "bridge" {
			if err := generateBridgeFile(gen, file); err != nil {
				return fmt.Errorf("%s bridge: %w", file.Desc.Path(), err)
			}
		}
	}

	resp := gen.Response()
	out, err := proto.Marshal(resp)
	if err != nil {
		return err
	}
	_, err = os.Stdout.Write(out)
	return err
}

func injectGoPackage(f *descriptorpb.FileDescriptorProto) {
	filePath := f.GetName()
	base := strings.TrimSuffix(filePath, ".proto")
	base = strings.ReplaceAll(base, "\\", "/")
	goPkg := "music.lyric/" + base + ";" + path.Base(base)
	if f.Options == nil {
		f.Options = &descriptorpb.FileOptions{}
	}
	if f.Options.GoPackage != nil && *f.Options.GoPackage != "" {
		return
	}
	f.Options.GoPackage = proto.String(goPkg)
}
