# music-lyric-model-cpp

[English](./README.md) | [简体中文](./README.zh-Hans.md) | [繁體中文](./README.zh-Hant.md)

**music lyric model** 的 C++ 绑定.

## 使用

将本仓库作为 submodule 引入, 并在你的 CMake 工程中链接:

```bash
git submodule add https://github.com/music-lyric/music-lyric-model-cpp.git third_party/music-lyric-model-cpp
```

```cmake
add_subdirectory(third_party/music-lyric-model-cpp)
target_link_libraries(your_app PRIVATE music_lyric_model)
```

```cpp
#include "music_lyric_model.h"

// makeInfo 会盖上当前的 SCHEMA_VERSION.
music_lyric_model::runtime::proto::Info info = music_lyric_model::runtime::makeInfo();
info.set_type(music_lyric_model::runtime::proto::INFO_TYPE_VALID);

std::vector<uint8_t> bytes = music_lyric_model::runtime::encodeInfo(info);
std::string          json  = music_lyric_model::runtime::infoToJson(info);

auto fromBytes = music_lyric_model::runtime::decodeInfo(bytes);
auto fromJson  = music_lyric_model::runtime::infoFromJson(json);
```

## 构建

### 环境要求

- CMake ≥ 3.21
- Ninja

### CMake

配置并构建:

```bash
cmake --preset default
cmake --build --preset debug      # 或: --preset release
```

### Make

从 proto submodule 重新生成 `gen/` (需要 `buf`):

```bash
git submodule update --init --recursive   # 拉取 proto/
make gen
```

`make` 同样封装了构建: `make build`, `make clean`.

## 许可证

基于 [MIT License](./LICENSE) 发布.
