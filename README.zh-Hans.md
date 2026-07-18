# music-lyric-model-cpp

[English](./README.md) | [简体中文](./README.zh-Hans.md) | [繁體中文](./README.zh-Hant.md)

**music lyric model** 的 C++ 绑定.

## 使用

将本仓库作为 submodule 引入, 并在你的 CMake 工程中链接:

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
word.time    = makeTime({.start = 0, .end = 500});

LineNormal line;
line.time  = makeTime({.start = 0, .end = 1000});
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

### 命名空间

| 命名空间                     | 作用             |
| ---------------------------- | ---------------- |
| `music_lyric_model::common`  | 共享原语         |
| `music_lyric_model::parsed`  | 解析产物         |
| `music_lyric_model::storage` | 持久化           |
| `music_lyric_model`          | `SCHEMA_VERSION` |

`#include "music_lyric_model.h"` 会引入上述三个模块.

`parsed` 与 `storage` 依赖 `common` 的共享类型与 helper, 彼此不互相包含.

## 构建

### 环境要求

- CMake ≥ 3.21
- Ninja
- C++20 编译器

### CMake

配置并构建:

```bash
cmake --preset default
cmake --build --preset debug
```

运行测试:

```bash
make test
```

### Make

从 proto submodule 重新生成 `private/pb/`、`src/model/**/*.gen.h` 与 `private/bridge/` (需要 `buf` 与 Go):

```bash
git submodule update --init --recursive
make gen
```

`make` 同样封装了构建: `make build`, `make format`, `make clean`.

## 许可证

基于 [MIT License](./LICENSE) 发布.
