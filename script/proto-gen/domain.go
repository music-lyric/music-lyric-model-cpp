package main

import (
	"fmt"
	"sort"
	"strings"

	"google.golang.org/protobuf/compiler/protogen"
	"google.golang.org/protobuf/reflect/protoreflect"
)

type includeSet struct {
	system map[string]struct{}
	local  map[string]struct{}
}

/**
 * Creates an empty include set.
 */
func newIncludeSet() *includeSet {
	return &includeSet{
		system: map[string]struct{}{},
		local:  map[string]struct{}{},
	}
}

/**
 * Records a system header include.
 */
func (s *includeSet) addSystem(h string) {
	s.system[h] = struct{}{}
}

/**
 * Records a project-relative local include.
 */
func (s *includeSet) addLocal(h string) {
	s.local[h] = struct{}{}
}

/**
 * Renders includes in a stable order: system first, then local.
 */
func (s *includeSet) render() string {
	var sys, loc []string
	for h := range s.system {
		sys = append(sys, h)
	}
	for h := range s.local {
		loc = append(loc, h)
	}
	sort.Strings(sys)
	sort.Strings(loc)
	var b strings.Builder
	for _, h := range sys {
		fmt.Fprintf(&b, "#include <%s>\n", h)
	}
	if len(sys) > 0 && len(loc) > 0 {
		b.WriteByte('\n')
	}
	for _, h := range loc {
		fmt.Fprintf(&b, "#include \"%s\"\n", h)
	}
	return b.String()
}

/**
 * Domain type string for a field as used inside a struct (not repeated/map wrapper).
 */
func domainElementType(f *protogen.Field, selfPkg string, incs *includeSet) string {
	switch f.Desc.Kind() {
	case protoreflect.MessageKind:
		msg := f.Message
		if isMap(f) {
			// Map entry key/value handled elsewhere.
			return ""
		}
		pkg := string(msg.Desc.ParentFile().Package())
		name := string(msg.Desc.Name())
		if pkg == selfPkg {
			return name
		}
		return domainNamespace(pkg) + "::" + name
	case protoreflect.EnumKind:
		en := f.Enum
		pkg := string(en.Desc.ParentFile().Package())
		name := string(en.Desc.Name())
		if pkg == selfPkg {
			return name
		}
		return domainNamespace(pkg) + "::" + name
	default:
		t := scalarCppType(f.Desc.Kind())
		if t == "std::string" {
			incs.addSystem("string")
		} else if strings.HasSuffix(t, "_t") {
			incs.addSystem("cstdint")
		}
		return t
	}
}

/**
 * Full domain field type including optional/vector/map wrappers.
 */
func domainFieldType(f *protogen.Field, selfPkg string, selfPath string, incs *includeSet) string {
	if isMap(f) {
		key := f.Message.Fields[0]
		val := f.Message.Fields[1]
		kt := domainElementType(key, selfPkg, incs)
		vt := domainElementType(val, selfPkg, incs)
		// Foreign message value includes.
		if val.Desc.Kind() == protoreflect.MessageKind {
			addDomainDep(val.Message, selfPath, incs)
		}
		if key.Desc.Kind() == protoreflect.MessageKind {
			addDomainDep(key.Message, selfPath, incs)
		}
		if kt == "std::string" || vt == "std::string" {
			incs.addSystem("string")
		}
		incs.addSystem("unordered_map")
		return fmt.Sprintf("std::unordered_map<%s, %s>", kt, vt)
	}

	elem := ""
	switch f.Desc.Kind() {
	case protoreflect.MessageKind:
		addDomainDep(f.Message, selfPath, incs)
		elem = domainElementType(f, selfPkg, incs)
	case protoreflect.EnumKind:
		addEnumDep(f.Enum, selfPath, incs)
		elem = domainElementType(f, selfPkg, incs)
	default:
		elem = domainElementType(f, selfPkg, incs)
	}

	if isRepeated(f) {
		incs.addSystem("vector")
		return "std::vector<" + elem + ">"
	}

	// optional scalar
	if isExplicitOptional(f) {
		incs.addSystem("optional")
		return "std::optional<" + elem + ">"
	}

	// message field -> optional, except bare document containers (meta).
	if isOptionalMessage(f) {
		if isBareMessageField(f) {
			return elem
		}
		incs.addSystem("optional")
		return "std::optional<" + elem + ">"
	}

	return elem
}

/**
 * Returns true when a singular message field should be a bare value, not optional.
 * Matches the public API choice for document-level Meta containers.
 */
func isBareMessageField(f *protogen.Field) bool {
	if f.Desc.Kind() != protoreflect.MessageKind || f.Desc.IsList() || f.Desc.IsMap() {
		return false
	}
	return string(f.Desc.Name()) == "meta"
}

/**
 * Adds a local include for a message defined in another file.
 */
func addDomainDep(m *protogen.Message, selfPath string, incs *includeSet) {
	path := m.Desc.ParentFile().Path()
	if path == selfPath {
		return
	}
	incs.addLocal("model/" + strings.TrimSuffix(path, ".proto") + ".gen.h")
}

/**
 * Adds a local include for an enum defined in another file.
 */
func addEnumDep(e *protogen.Enum, selfPath string, incs *includeSet) {
	path := e.Desc.ParentFile().Path()
	if path == selfPath {
		return
	}
	incs.addLocal("model/" + strings.TrimSuffix(path, ".proto") + ".gen.h")
}

/**
 * Default initializer for a domain field, or empty if none.
 */
func domainFieldDefault(f *protogen.Field) string {
	if isMap(f) || isRepeated(f) || isExplicitOptional(f) || (isOptionalMessage(f) && !isBareMessageField(f)) {
		return ""
	}
	switch f.Desc.Kind() {
	case protoreflect.BoolKind:
		return " = false"
	case protoreflect.EnumKind:
		return " = " + string(f.Enum.Desc.Name()) + "::Unspecified"
	case protoreflect.Int32Kind, protoreflect.Sint32Kind, protoreflect.Sfixed32Kind,
		protoreflect.Uint32Kind, protoreflect.Fixed32Kind,
		protoreflect.Int64Kind, protoreflect.Sint64Kind, protoreflect.Sfixed64Kind,
		protoreflect.Uint64Kind, protoreflect.Fixed64Kind,
		protoreflect.FloatKind, protoreflect.DoubleKind:
		return " = 0"
	default:
		return ""
	}
}

/**
 * Special-case: Timing lives in domain as common::Timing inside common.h historically,
 * but we generate it into timing.gen.h. Enum defaults use Unspecified when present.
 */
func hasUnspecified(e *protogen.Enum) bool {
	for _, v := range e.Values {
		if domainEnumValueName(e, v) == "Unspecified" {
			return true
		}
	}
	return false
}

/**
 * Emits a domain model header for one proto file.
 */
func generateDomainFile(gen *protogen.Plugin, file *protogen.File) error {
	if len(file.Messages) == 0 && len(file.Enums) == 0 {
		return nil
	}

	selfPath := file.Desc.Path()
	selfPkg := string(file.Desc.Package())
	outPath := domainOutPath(selfPath)
	// Place the file at the proto-relative path; import path is only for protogen bookkeeping.
	g := gen.NewGeneratedFile(outPath, file.GoImportPath)

	guard := domainIncludeGuard(selfPath)
	ns := domainNamespace(selfPkg)

	incs := newIncludeSet()

	// Pre-scan to populate includes (we render body first into a buffer).
	var body strings.Builder
	writeComments := func(c protogen.Comments, indent string) {
		lines := commentLines(c)
		if len(lines) == 0 {
			return
		}
		fmt.Fprintf(&body, "%s/**\n", indent)
		for _, line := range lines {
			if line == "" {
				fmt.Fprintf(&body, "%s *\n", indent)
			} else {
				fmt.Fprintf(&body, "%s * %s\n", indent, line)
			}
		}
		fmt.Fprintf(&body, "%s */\n", indent)
	}

	// Enums first (top-level).
	for _, e := range file.Enums {
		writeComments(e.Comments.Leading, "\t")
		fmt.Fprintf(&body, "\tenum class %s {\n", e.Desc.Name())
		for i, v := range e.Values {
			writeComments(v.Comments.Leading, "\t\t")
			name := domainEnumValueName(e, v)
			fmt.Fprintf(&body, "\t\t%s = %d", name, v.Desc.Number())
			if i+1 < len(e.Values) {
				body.WriteString(",\n")
			} else {
				body.WriteString(",\n")
			}
		}
		body.WriteString("\t};\n\n")
	}

	// Messages. Nested messages are emitted before outer ones that reference them —
	// we emit in declaration order as listed; for pure nested, walk recursively.
	var emitMessage func(m *protogen.Message)
	emitMessage = func(m *protogen.Message) {
		// Nested types first.
		for _, nested := range m.Messages {
			if nested.Desc.IsMapEntry() {
				continue
			}
			emitMessage(nested)
		}
		for _, e := range m.Enums {
			writeComments(e.Comments.Leading, "\t")
			fmt.Fprintf(&body, "\tenum class %s {\n", e.Desc.Name())
			for _, v := range e.Values {
				writeComments(v.Comments.Leading, "\t\t")
				fmt.Fprintf(&body, "\t\t%s = %d,\n", domainEnumValueName(e, v), v.Desc.Number())
			}
			body.WriteString("\t};\n\n")
		}

		if isVariantWrapper(m) {
			// Collect arm types.
			oo := m.Oneofs[0]
			var arms []string
			for _, f := range oo.Fields {
				if f.Desc.Kind() != protoreflect.MessageKind {
					// Scalar oneof arm — rare; model as raw type.
					arms = append(arms, domainFieldType(f, selfPkg, selfPath, incs))
					continue
				}
				addDomainDep(f.Message, selfPath, incs)
				arms = append(arms, domainElementType(f, selfPkg, incs))
			}
			incs.addSystem("variant")
			writeComments(m.Comments.Leading, "\t")
			fmt.Fprintf(&body, "\tusing %s = std::variant<%s>;\n\n", m.Desc.Name(), strings.Join(arms, ", "))
			return
		}

		writeComments(m.Comments.Leading, "\t")
		fmt.Fprintf(&body, "\tstruct %s {\n", m.Desc.Name())

		// Fields in field-number order (protogen already orders by number).
		// Skip fields that belong to a non-synthetic oneof if we ever mix; here all non-variant.
		for _, f := range m.Fields {
			if f.Oneof != nil && !f.Oneof.Desc.IsSynthetic() {
				// Mixed oneof not supported as variant wrapper; skip with comment.
				fmt.Fprintf(&body, "\t\t// unsupported mixed oneof field: %s\n", f.Desc.Name())
				continue
			}
			writeComments(f.Comments.Leading, "\t\t")
			typ := domainFieldType(f, selfPkg, selfPath, incs)
			name := domainFieldName(f)
			def := domainFieldDefault(f)
			// Fix enum default only when Unspecified exists.
			if f.Desc.Kind() == protoreflect.EnumKind && !isMap(f) && !isRepeated(f) && !isExplicitOptional(f) {
				if hasUnspecified(f.Enum) {
					en := domainElementType(f, selfPkg, incs)
					def = " = " + en + "::Unspecified"
				} else {
					def = ""
				}
			}
			fmt.Fprintf(&body, "\t\t%s %s%s;\n", typ, name, def)
		}
		body.WriteString("\t};\n\n")
	}

	for _, m := range file.Messages {
		if m.Desc.IsMapEntry() {
			continue
		}
		emitMessage(m)
	}

	// Special include: Timing historically in common.h — we generate timing.gen.h standalone.
	// When domain fields reference Timing enum from another file, addDomainDep covers it.

	// Force common includes used by almost all.
	// string often needed.
	_ = incs

	// Header skeleton.
	g.P("// Code generated by protoc-gen-music-lyric-cpp. DO NOT EDIT.")
	g.P("// source: ", selfPath)
	g.P()
	g.P("#ifndef ", guard)
	g.P("#define ", guard)
	g.P()
	incText := incs.render()
	if incText != "" {
		g.P(strings.TrimRight(incText, "\n"))
		g.P()
	}
	g.P("namespace ", ns, " {")
	// body already has trailing newline; trim final extra blank
	content := strings.TrimRight(body.String(), "\n")
	if content != "" {
		g.P(content)
	}
	g.P("} // namespace ", ns)
	g.P()
	g.P("#endif")
	return nil
}

/**
 * Builds an include guard macro from a proto path.
 */
func domainIncludeGuard(path string) string {
	p := strings.TrimSuffix(path, ".proto")
	p = strings.ReplaceAll(p, "/", "_")
	p = strings.ReplaceAll(p, "-", "_")
	p = strings.ReplaceAll(p, ".", "_")
	return "MUSIC_LYRIC_MODEL_" + strings.ToUpper(p) + "_GEN_H_"
}
