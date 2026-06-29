# music-lyric-model-cpp

[English](./README.md) | [简体中文](./README.zh-Hans.md) | [繁體中文](./README.zh-Hant.md)

The C++ binding for the **music lyric model**.

## Usage

Add this repository as a submodule and link it from your CMake project:

```bash
git submodule add https://github.com/music-lyric/music-lyric-model-cpp.git third_party/music-lyric-model-cpp
```

```cmake
add_subdirectory(third_party/music-lyric-model-cpp)
target_link_libraries(your_app PRIVATE music_lyric_model)
```

```cpp
#include "music_lyric_model.h"

// makeInfo stamps the current SCHEMA_VERSION.
auto info = music_lyric_model::makeInfo();
info.set_type(music_lyric_model::INFO_TYPE_NORMAL);

std::vector<uint8_t> bytes = music_lyric_model::encodeInfo(info);
std::string          json  = music_lyric_model::infoToJson(info);

auto fromBytes = music_lyric_model::decodeInfo(bytes);
auto fromJson  = music_lyric_model::infoFromJson(json);
```

## Build

### Requirements

- CMake ≥ 3.21
- Ninja

### CMake

Configure and build:

```bash
cmake --preset default
cmake --build --preset debug      # or: --preset release
```

### Make

Regenerate `gen/` from the proto submodule (requires `buf`):

```bash
git submodule update --init --recursive   # fetch proto/
make gen
```

`make` also wraps the build: `make build`, `make clean`.

## License

Released under the [MIT License](./LICENSE).
