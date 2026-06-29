import os
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

SRC = os.path.join(ROOT, "src")

EXCLUDES = {"version.h"}

EXTENSIONS = (".cpp", ".h")


def main() -> None:
    """Format every hand-written source under src/ in place with clang-format."""
    clang_format = shutil.which("clang-format")
    if clang_format is None:
        sys.exit("clang-format not found on PATH")

    files = []
    for dirpath, _, filenames in os.walk(SRC):
        for name in filenames:
            if name in EXCLUDES or not name.endswith(EXTENSIONS):
                continue
            files.append(os.path.join(dirpath, name))
    files.sort()

    if not files:
        print("no sources found under src/")
        return

    subprocess.run([clang_format, "-i", *files], check=True)
    print(f"formatted {len(files)} files under src/")


if __name__ == "__main__":
    main()
