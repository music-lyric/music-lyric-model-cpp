cmake_minimum_required(VERSION 3.21)

project(music_lyric_model VERSION 3.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Embed debug info in objects (/Z7) instead of /Zi to avoid PDB write contention
foreach(_flags CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    string(REPLACE "/Zi" "/Z7" ${_flags} "${${_flags}}")
endforeach()

# protobuf pinned to exactly 7.35.1 (git v35.1)
find_package(Protobuf 7.35.1 EXACT CONFIG QUIET)
if(NOT Protobuf_FOUND)
    include(FetchContent)
    set(protobuf_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(protobuf_INSTALL OFF CACHE BOOL "" FORCE)
    set(protobuf_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(protobuf_MSVC_STATIC_RUNTIME OFF CACHE BOOL "" FORCE)
    set(protobuf_BUILD_PROTOC_BINARIES OFF CACHE BOOL "" FORCE)
    set(protobuf_BUILD_LIBUPB OFF CACHE BOOL "" FORCE)
    set(protobuf_BUILD_CONFORMANCE OFF CACHE BOOL "" FORCE)
    set(protobuf_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(protobuf_WITH_ZLIB OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(protobuf
        GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
        GIT_TAG v35.1
        GIT_SHALLOW TRUE
        GIT_SUBMODULES_RECURSE TRUE
    )
    FetchContent_MakeAvailable(protobuf)
endif()

file(GLOB_RECURSE MUSIC_LYRIC_GEN_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/gen/*.pb.cc")
# Temporary: common + parsed until storage is ported to proto v4.
file(GLOB_RECURSE MUSIC_LYRIC_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/parsed/*.cpp"
)

set(MUSIC_LYRIC_PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/music_lyric_model.h")

add_library(music_lyric_model STATIC ${MUSIC_LYRIC_GEN_SOURCES} ${MUSIC_LYRIC_SOURCES} ${MUSIC_LYRIC_PUBLIC_HEADER})
add_library(music_lyric::model ALIAS music_lyric_model)

target_compile_features(music_lyric_model PUBLIC cxx_std_17)

# MSVC reads source as the system code page (e.g. GBK) by default, but abseil/protobuf headers are UTF-8.
# /bigobj is required by protobuf gencode.
target_compile_options(music_lyric_model PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/utf-8;/bigobj>)

target_include_directories(music_lyric_model
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/gen"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_include_directories(music_lyric_model SYSTEM INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/gen"
)

target_link_libraries(music_lyric_model PUBLIC protobuf::libprotobuf)

if(PROJECT_IS_TOP_LEVEL)
    enable_testing()

    # Temporary: common + parsed tests until storage is ported to proto v4.
    file(GLOB_RECURSE MUSIC_LYRIC_TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/common/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/parsed/*.cpp"
    )

    add_executable(music_lyric_model_test ${MUSIC_LYRIC_TEST_SOURCES})
    target_link_libraries(music_lyric_model_test PRIVATE music_lyric::model)
    target_include_directories(music_lyric_model_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test")

    add_test(NAME music_lyric_model_test COMMAND music_lyric_model_test)
endif()
