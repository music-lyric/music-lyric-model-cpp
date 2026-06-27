.PHONY: gen configure build clean

# Regenerate gen/ from the proto submodule (requires buf).
gen:
	buf generate proto --template buf.gen.yml

# Configure the CMake build tree into build/.
configure:
	cmake --preset default

# Build the library (Debug).
build: configure
	cmake --build --preset debug

# Remove the build tree.
clean:
	rm -rf build
