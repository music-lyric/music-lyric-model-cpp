.PHONY: gen configure build format clean

# Regenerate gen/ and src/version.h from the proto submodule (requires buf).
gen:
	buf generate proto --template buf.gen.yml
	python script/generate-proto-version.py

# Configure the CMake build tree into build/.
configure:
	cmake --preset default

# Build the library (Debug).
build: configure
	cmake --build --preset debug

# Format all hand-written sources under src/ in place (requires clang-format).
format:
	python script/format-code.py

# Remove the build tree.
clean:
	rm -rf build
