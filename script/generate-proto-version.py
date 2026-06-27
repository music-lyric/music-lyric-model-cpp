import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

VERSION_TXT = os.path.join(ROOT, "proto", "src", "version.txt")

VERSION_H = os.path.join(ROOT, "src", "version.h")


def main() -> None:
    """Read the schema version and write it into the generated header."""
    with open(VERSION_TXT, encoding="utf-8") as f:
        version = f.read().strip()
    content = (
        "#ifndef MUSIC_LYRIC_VERSION_H_\n"
        "#define MUSIC_LYRIC_VERSION_H_\n"
        "\n"
        "namespace music_lyric_model {\n"
        "\t/**\n"
        "\t * Schema version of the proto this binding targets.\n"
        "\t * Generated from proto/src/version.txt by script/generate-proto-version.py.\n"
        "\t */\n"
        f'\tinline constexpr const char *SCHEMA_VERSION = "{version}";\n'
        "}\n"
        "\n"
        "#endif\n"
    )
    with open(VERSION_H, "w", encoding="utf-8") as f:
        f.write(content)
    print(f"wrote {VERSION_H} (SCHEMA_VERSION = {version})")


if __name__ == "__main__":
    main()
