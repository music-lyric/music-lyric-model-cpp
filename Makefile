BUILD_DIR ?= build

.PHONY: gen configure build clean

# Regenerate gen/ from the proto submodule.
gen:
	buf generate proto --template buf.gen.yml

# Configure the CMake build tree.
configure:
	cmake -S . -B $(BUILD_DIR)

# Build the library.
build: configure
	cmake --build $(BUILD_DIR)

# Remove the build tree.
clean:
	rm -rf $(BUILD_DIR)
