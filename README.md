# music-lyric-model-cpp

[English](./README.md) | [简体中文](./README.zh-Hans.md) | [繁體中文](./README.zh-Hant.md)

The C++ binding for the **music lyric model**.

## Usage

Add this repository as a submodule and link it from your CMake project:

```bash
git submodule add https://github.com/music-lyric/music-lyric-model-cpp.git third-party/music-lyric-model-cpp
```

```cmake
add_subdirectory(third-party/music-lyric-model-cpp)
target_link_libraries(your_app PRIVATE music_lyric::model)

# recommended for release: binary only + LTO
# set(MUSIC_LYRIC_MODEL_ENABLE_JSON OFF CACHE BOOL "" FORCE)
# set(MUSIC_LYRIC_MODEL_ENABLE_LTO ON CACHE BOOL "" FORCE)
# add_subdirectory(...)
# set_property(TARGET your_app PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
```

```cpp
#include "music_lyric_model.h"

using namespace music_lyric_model;
using namespace music_lyric_model::common;
using namespace music_lyric_model::parsed;
using namespace music_lyric_model::storage;

// Parsed
Info info = makeParsedInfo();
info.type   = InfoType::Valid;
info.timing = Timing::Word;

WordNormal word;
word.content = "hello";
word.time    = makeTime({.start = 0, .end = 500});

LineNormal line;
line.time  = makeTime({.start = 0, .end = 1000});
line.words = {makeWordNormal(std::move(word))};
info.lines.push_back(makeParsedLineNormal(std::move(line)));

std::vector<uint8_t> bytes = encodeParsedInfo(info);
Info fromBytes = decodeParsedInfo(bytes);

// Storage
Lyric lyric = makeStorageLyric();
lyric.timing = Timing::Word;
std::vector<uint8_t> lyricBytes = encodeStorageLyric(lyric);

#if MUSIC_LYRIC_MODEL_ENABLE_JSON
std::string json = parsedInfoToJson(info);
Info fromJson = parsedInfoFromJson(json);
#endif
```

### Namespaces

| Namespace                    | Role              |
| ---------------------------- | ----------------- |
| `music_lyric_model::common`  | Shared primitives |
| `music_lyric_model::parsed`  | Parse output      |
| `music_lyric_model::storage` | Persistence       |
| `music_lyric_model`          | `SCHEMA_VERSION`  |

`#include "music_lyric_model.h"` pulls in all three modules.

`parsed` and `storage` depend on `common` for shared types and helpers; they do not include each other.

## Build

### Requirements

- CMake ≥ 3.21
- Ninja
- A C++20 compiler

### CMake options

| Option                          | Default | Role                                                     |
| ------------------------------- | ------- | -------------------------------------------------------- |
| `MUSIC_LYRIC_MODEL_ENABLE_JSON` | `ON`    | `*ToJson` / `*FromJson` (pulls `json_util`)              |
| `MUSIC_LYRIC_MODEL_ENABLE_LTO`  | `OFF`   | LTO/IPO on this library (and top-level test) for Release |

Players that only accept protobuf binary should set `MUSIC_LYRIC_MODEL_ENABLE_JSON=OFF` and enable LTO on the final executable (and preferably this option too).

### CMake

Configure and build:

```bash
cmake --preset default
cmake --build --preset debug
cmake --build --preset release
```

Player-oriented configure (JSON off, LTO on):

```bash
cmake --preset player
cmake --build --preset player
```

Run tests:

```bash
make test
# or: ctest --test-dir build -C Release
```

### Make

Regenerate `private/pb/`, `src/model/**/*.gen.h`, and `private/bridge/` from the proto submodule (requires `buf` and Go):

```bash
git submodule update --init --recursive
make gen
```

`make` also wraps the build: `make build`, `make format`, `make clean`.

## License

Released under the [MIT License](./LICENSE).
