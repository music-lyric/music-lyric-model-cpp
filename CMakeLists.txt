cmake_minimum_required(VERSION 3.21)

project(music_lyric_model VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# protobuf pinned to exactly 7.35.1 (git v35.1)
find_package(Protobuf 7.35.1 EXACT CONFIG QUIET)
if(NOT Protobuf_FOUND)
    include(FetchContent)
    set(protobuf_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(protobuf_INSTALL OFF CACHE BOOL "" FORCE)
    set(protobuf_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(protobuf
        GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
        GIT_TAG v35.1
        GIT_SHALLOW TRUE
        GIT_SUBMODULES_RECURSE TRUE
    )
    FetchContent_MakeAvailable(protobuf)
endif()

file(GLOB_RECURSE MUSIC_LYRIC_GEN_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/gen/*.pb.cc")
file(GLOB_RECURSE MUSIC_LYRIC_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(music_lyric_model ${MUSIC_LYRIC_GEN_SOURCES} ${MUSIC_LYRIC_SOURCES})
add_library(music_lyric::model ALIAS music_lyric_model)

target_compile_features(music_lyric_model PUBLIC cxx_std_17)

target_include_directories(music_lyric_model PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gen>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:include>"
)

target_link_libraries(music_lyric_model PUBLIC protobuf::libprotobuf)
