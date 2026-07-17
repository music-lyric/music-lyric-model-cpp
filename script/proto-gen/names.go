package main

import (
	"strings"
	"unicode"
	"unicode/utf8"

	"google.golang.org/protobuf/compiler/protogen"
	"google.golang.org/protobuf/reflect/protoreflect"
)

/**
 * Converts a snake_case identifier to lowerCamelCase.
 */
func snakeToLowerCamel(s string) string {
	parts := strings.Split(s, "_")
	if len(parts) == 0 {
		return s
	}
	var b strings.Builder
	for i, p := range parts {
		if p == "" {
			continue
		}
		if i == 0 {
			b.WriteString(strings.ToLower(p))
			continue
		}
		b.WriteString(titleASCII(p))
	}
	return b.String()
}

/**
 * Converts a SCREAMING_SNAKE token to PascalCase.
 */
func screamingToPascal(s string) string {
	parts := strings.Split(strings.ToLower(s), "_")
	var b strings.Builder
	for _, p := range parts {
		if p == "" {
			continue
		}
		b.WriteString(titleASCII(p))
	}
	return b.String()
}

/**
 * Titles an ASCII word: first letter upper, rest lower.
 */
func titleASCII(s string) string {
	if s == "" {
		return s
	}
	r, size := utf8.DecodeRuneInString(s)
	if r == utf8.RuneError && size == 1 {
		return strings.ToUpper(s[:1]) + strings.ToLower(s[1:])
	}
	return string(unicode.ToUpper(r)) + strings.ToLower(s[size:])
}

/**
 * Returns the C++ namespace for a proto package, e.g. lyric.common -> music_lyric_model::common.
 */
func domainNamespace(pkg string) string {
	if !strings.HasPrefix(pkg, "lyric.") {
		return cfg.moduleNS
	}
	rest := strings.TrimPrefix(pkg, "lyric.")
	parts := strings.Split(rest, ".")
	return cfg.moduleNS + "::" + strings.Join(parts, "::")
}

/**
 * Returns the protobuf C++ namespace for a proto package, e.g. lyric.common -> lyric::common.
 */
func pbNamespace(pkg string) string {
	return strings.ReplaceAll(pkg, ".", "::")
}

/**
 * Strips a conventional enum value prefix (TYPE_NAME_) from an enum value name.
 * Example: enum AgentType + AGENT_TYPE_PERSON -> PERSON.
 */
func stripEnumPrefix(enum *protogen.Enum, value string) string {
	prefix := toScreamingSnake(string(enum.Desc.Name())) + "_"
	if strings.HasPrefix(value, prefix) {
		return strings.TrimPrefix(value, prefix)
	}
	// Fallback: ENUMNAME_ without camel split.
	alt := strings.ToUpper(string(enum.Desc.Name())) + "_"
	if strings.HasPrefix(value, alt) {
		return strings.TrimPrefix(value, alt)
	}
	return value
}

/**
 * Converts PascalCase to SCREAMING_SNAKE_CASE.
 * Handles acronym boundaries: AgentType -> AGENT_TYPE, XMLParser -> XML_PARSER.
 */
func toScreamingSnake(s string) string {
	runes := []rune(s)
	var b strings.Builder
	for i, r := range runes {
		if i > 0 {
			prev := runes[i-1]
			nextLower := i+1 < len(runes) && unicode.IsLower(runes[i+1])
			if unicode.IsUpper(r) && (unicode.IsLower(prev) || (unicode.IsUpper(prev) && nextLower)) {
				b.WriteByte('_')
			}
		}
		b.WriteRune(unicode.ToUpper(r))
	}
	return b.String()
}

/**
 * Domain enum enumerator identifier for a proto enum value.
 */
func domainEnumValueName(enum *protogen.Enum, v *protogen.EnumValue) string {
	raw := string(v.Desc.Name())
	stripped := stripEnumPrefix(enum, raw)
	return screamingToPascal(stripped)
}

/**
 * Domain field name for a proto field (snake_case -> lowerCamelCase).
 */
func domainFieldName(f *protogen.Field) string {
	return snakeToLowerCamel(string(f.Desc.Name()))
}

/**
 * Protobuf C++ accessor base name (snake_case field name).
 */
func pbFieldName(f *protogen.Field) string {
	return string(f.Desc.Name())
}

/**
 * Oneof case enum constant as emitted by the C++ generator, e.g. kNormal.
 */
func pbOneofCaseName(f *protogen.Field) string {
	name := string(f.Desc.Name())
	parts := strings.Split(name, "_")
	var b strings.Builder
	b.WriteString("k")
	for _, p := range parts {
		if p == "" {
			continue
		}
		b.WriteString(titleASCII(p))
	}
	return b.String()
}

/**
 * Include path for a generated domain header from a proto file path.
 */
func domainHeaderPath(f *protogen.File) string {
	// common/time.proto -> model/common/time.gen.h
	name := f.Desc.Path()
	name = strings.TrimSuffix(name, ".proto")
	return "model/" + name + ".gen.h"
}

/**
 * Include path for a generated bridge header from a proto file path.
 */
func bridgeHeaderPath(f *protogen.File) string {
	// common/time.proto -> common/time.gen.h under private/bridge
	name := f.Desc.Path()
	name = strings.TrimSuffix(name, ".proto")
	return name + ".gen.h"
}

/**
 * Domain output path relative to the domain plugin out root (src/).
 */
func domainOutPath(protoPath string) string {
	return strings.TrimSuffix(protoPath, ".proto") + ".gen.h"
}

/**
 * Bridge output path relative to the bridge plugin out root (private/bridge/).
 */
func bridgeOutPath(protoPath string) string {
	return strings.TrimSuffix(protoPath, ".proto") + ".gen.h"
}

/**
 * Relative include used by domain headers to pull in another domain type.
 */
func domainIncludeForMessage(m *protogen.Message) string {
	path := m.Location.SourceFile
	if path == "" {
		// Fall back to the parent file of the message descriptor.
		path = m.Desc.ParentFile().Path()
	}
	path = strings.TrimSuffix(path, ".proto")
	return "model/" + path + ".gen.h"
}

/**
 * Relative include for a protobuf C++ header.
 */
func pbIncludeForFile(path string) string {
	path = strings.TrimSuffix(path, ".proto")
	return path + ".pb.h"
}

/**
 * Maps a protobuf scalar kind to a C++ type string.
 */
func scalarCppType(kind protoreflect.Kind) string {
	switch kind {
	case protoreflect.BoolKind:
		return "bool"
	case protoreflect.Int32Kind, protoreflect.Sint32Kind, protoreflect.Sfixed32Kind:
		return "int32_t"
	case protoreflect.Uint32Kind, protoreflect.Fixed32Kind:
		return "uint32_t"
	case protoreflect.Int64Kind, protoreflect.Sint64Kind, protoreflect.Sfixed64Kind:
		return "int64_t"
	case protoreflect.Uint64Kind, protoreflect.Fixed64Kind:
		return "uint64_t"
	case protoreflect.FloatKind:
		return "float"
	case protoreflect.DoubleKind:
		return "double"
	case protoreflect.StringKind, protoreflect.BytesKind:
		return "std::string"
	default:
		return "void"
	}
}

/**
 * Returns true when a field is a map.
 */
func isMap(f *protogen.Field) bool {
	return f.Desc.IsMap()
}

/**
 * Returns true when a field is repeated and not a map.
 */
func isRepeated(f *protogen.Field) bool {
	return f.Desc.IsList()
}

/**
 * Returns true when a field uses explicit optional presence (proto3 optional).
 */
func isExplicitOptional(f *protogen.Field) bool {
	return f.Desc.HasOptionalKeyword()
}

/**
 * Returns true when a message field should be modeled as std::optional in domain types.
 * Message fields always have presence in proto3.
 */
func isOptionalMessage(f *protogen.Field) bool {
	return f.Desc.Kind() == protoreflect.MessageKind && !f.Desc.IsList() && !f.Desc.IsMap()
}

/**
 * True when the message has a single oneof and every field is in that oneof (pure variant wrapper).
 */
func isVariantWrapper(m *protogen.Message) bool {
	if len(m.Oneofs) != 1 {
		return false
	}
	oo := m.Oneofs[0]
	if oo.Desc.IsSynthetic() {
		return false
	}
	if len(oo.Fields) == 0 {
		return false
	}
	return len(m.Fields) == len(oo.Fields)
}

/**
 * Collects comments from a protogen comment set as a single trimmed block.
 */
func commentLines(c protogen.Comments) []string {
	raw := strings.TrimSpace(string(c))
	if raw == "" {
		return nil
	}
	var lines []string
	for _, line := range strings.Split(raw, "\n") {
		line = strings.TrimSpace(line)
		// Proto comments often start with a space after //.
		line = strings.TrimPrefix(line, " ")
		lines = append(lines, line)
	}
	return lines
}

/**
 * Include path for a bridge header as seen with -I private/.
 */
func bridgeIncludePath(protoPath string) string {
	return "bridge/" + strings.TrimSuffix(protoPath, ".proto") + ".gen.h"
}
