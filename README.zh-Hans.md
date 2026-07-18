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

# 推荐在发布版本中开启: binary only + LTO
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

### CMake 选项

| 选项                            | 默认  | 作用                                        |
| ------------------------------- | ----- | ------------------------------------------- |
| `MUSIC_LYRIC_MODEL_ENABLE_JSON` | `ON`  | `*ToJson` / `*FromJson`（依赖 `json_util`） |
| `MUSIC_LYRIC_MODEL_ENABLE_LTO`  | `OFF` | 对本库（及顶层测试）在 Release 开启 LTO/IPO |

仅接收 protobuf 二进制的 player 应设 `MUSIC_LYRIC_MODEL_ENABLE_JSON=OFF`，并在最终可执行文件上开启 LTO（建议同时打开本选项）。

### CMake

配置并构建:

```bash
cmake --preset default
cmake --build --preset debug
cmake --build --preset release
```

面向 player 的配置（关 JSON、开 LTO）:

```bash
cmake --preset player
cmake --build --preset player
```

运行测试:

```bash
make test
# 或: ctest --test-dir build -C Release
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
