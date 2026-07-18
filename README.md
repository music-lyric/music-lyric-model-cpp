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
word.time    = makeTime(Time{0, 500});

LineNormal line;
line.time  = makeTime(Time{0, 1000});
line.words = {makeWordNormal(std::move(word))};
info.lines.push_back(makeParsedLineNormal(std::move(line)));

std::vector<uint8_t> bytes = encodeParsedInfo(info);
std::string          json  = parsedInfoToJson(info);

Info fromBytes = decodeParsedInfo(bytes);
Info fromJson  = parsedInfoFromJson(json);

// Storage
Lyric lyric = makeStorageLyric();
lyric.timing = Timing::Word;
std::vector<uint8_t> lyricBytes = encodeStorageLyric(lyric);
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
- A C++17 compiler

### CMake

Configure and build:

```bash
cmake --preset default
cmake --build --preset debug
```

Run tests:

```bash
make test
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
