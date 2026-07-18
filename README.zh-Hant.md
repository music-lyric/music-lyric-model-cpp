# music-lyric-model-cpp

[English](./README.md) | [简体中文](./README.zh-Hans.md) | [繁體中文](./README.zh-Hant.md)

**music lyric model** 的 C++ 綁定.

## 使用

將本儲存庫作為 submodule 引入, 並在你的 CMake 專案中連結:

```bash
git submodule add https://github.com/music-lyric/music-lyric-model-cpp.git third-party/music-lyric-model-cpp
```

```cmake
add_subdirectory(third-party/music-lyric-model-cpp)
target_link_libraries(your_app PRIVATE music_lyric::model)

# 發布體積建議: binary only + LTO
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

### 命名空間

| 命名空間                     | 作用             |
| ---------------------------- | ---------------- |
| `music_lyric_model::common`  | 共享原語         |
| `music_lyric_model::parsed`  | 解析產物         |
| `music_lyric_model::storage` | 持久化           |
| `music_lyric_model`          | `SCHEMA_VERSION` |

`#include "music_lyric_model.h"` 會引入上述三個模組.

`parsed` 與 `storage` 依賴 `common` 的共享類型與 helper, 彼此不互相包含.

## 建置

### 環境需求

- CMake ≥ 3.21
- Ninja
- C++20 編譯器

### CMake 選項

| 選項                            | 預設  | 作用                                        |
| ------------------------------- | ----- | ------------------------------------------- |
| `MUSIC_LYRIC_MODEL_ENABLE_JSON` | `ON`  | `*ToJson` / `*FromJson`（依賴 `json_util`） |
| `MUSIC_LYRIC_MODEL_ENABLE_LTO`  | `OFF` | 對本庫（及頂層測試）在 Release 開啟 LTO/IPO |

僅接收 protobuf 二進位的 player 應設 `MUSIC_LYRIC_MODEL_ENABLE_JSON=OFF`，並在最終可執行檔上開啟 LTO（建議同時打開本選項）。

### CMake

設定並建置:

```bash
cmake --preset default
cmake --build --preset debug
cmake --build --preset release
```

面向 player 的設定（關 JSON、開 LTO）:

```bash
cmake --preset player
cmake --build --preset player
```

執行測試:

```bash
make test
# 或: ctest --test-dir build -C Release
```

### Make

從 proto submodule 重新產生 `private/pb/`、`src/model/**/*.gen.h` 與 `private/bridge/` (需要 `buf` 與 Go):

```bash
git submodule update --init --recursive
make gen
```

`make` 同樣封裝了建置: `make build`, `make format`, `make clean`.

## 授權

基於 [MIT License](./LICENSE) 發布.
