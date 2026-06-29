# music-lyric-model-cpp

[English](./README.md) | [简体中文](./README.zh-Hans.md) | [繁體中文](./README.zh-Hant.md)

**music lyric model** 的 C++ 綁定.

## 使用

將本儲存庫作為 submodule 引入, 並在你的 CMake 專案中連結:

```bash
git submodule add https://github.com/music-lyric/music-lyric-model-cpp.git third_party/music-lyric-model-cpp
```

```cmake
add_subdirectory(third_party/music-lyric-model-cpp)
target_link_libraries(your_app PRIVATE music_lyric_model)
```

```cpp
#include "music_lyric_model.h"

// makeInfo 會蓋上當前的 SCHEMA_VERSION.
auto info = music_lyric_model::makeInfo();
info.set_type(music_lyric_model::INFO_TYPE_NORMAL);

std::vector<uint8_t> bytes = music_lyric_model::encodeInfoWithBinary(info);
std::string          json  = music_lyric_model::encodeInfoWithJson(info);

auto fromBytes = music_lyric_model::decodeInfoWithBinary(bytes);
auto fromJson  = music_lyric_model::decodeInfoWithJson(json);
```

## 建置

### 環境需求

- CMake ≥ 3.21
- Ninja

### CMake

設定並建置:

```bash
cmake --preset default
cmake --build --preset debug      # 或: --preset release
```

### Make

從 proto submodule 重新產生 `gen/` (需要 `buf`):

```bash
git submodule update --init --recursive   # 拉取 proto/
make gen
```

`make` 同樣封裝了建置: `make build`, `make clean`.

## 授權

基於 [MIT License](./LICENSE) 發布.
