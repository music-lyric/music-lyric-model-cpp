.PHONY: gen configure build format test clean

# Regenerate protobuf C++ gencode, domain/bridge headers, and version.h.
gen:
	buf generate proto --template buf.gen.yml
	python script/generate-proto-version.py

# Configure cmake info.
configure:
	cmake --preset default

# Build the library.
build: configure
	cmake --build --preset debug

# Build and run the unit tests.
test: build
	ctest --test-dir build -C Debug --output-on-failure

# Format all sources.
format:
	python script/format-code.py

# Remove the build tree.
clean:
	rm -rf build
