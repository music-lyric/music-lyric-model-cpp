cmake_minimum_required(VERSION 3.21)

project(music_lyric_model VERSION 4.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MUSIC_LYRIC_MODEL_ENABLE_JSON "Build Protobuf Json Utils" ON)
option(MUSIC_LYRIC_MODEL_ENABLE_LTO "Enable LTO" OFF)

# Embed debug info in objects (/Z7) instead of /Zi to avoid PDB write contention
foreach(_flags CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    string(REPLACE "/Zi" "/Z7" ${_flags} "${${_flags}}")
endforeach()

if(MUSIC_LYRIC_MODEL_ENABLE_LTO)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
endif()

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

file(GLOB_RECURSE MUSIC_LYRIC_GEN_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/private/pb/*.pb.cc")
file(GLOB_RECURSE MUSIC_LYRIC_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/parsed/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/storage/*.cpp"
)

list(FILTER MUSIC_LYRIC_SOURCES EXCLUDE REGEX ".*/info_json\\.cpp$")
list(FILTER MUSIC_LYRIC_SOURCES EXCLUDE REGEX ".*/lyric_json\\.cpp$")
if(MUSIC_LYRIC_MODEL_ENABLE_JSON)
	list(APPEND MUSIC_LYRIC_SOURCES
		"${CMAKE_CURRENT_SOURCE_DIR}/src/parsed/info_json.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/storage/lyric_json.cpp"
	)
endif()

set(MUSIC_LYRIC_PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/music_lyric_model.h")

add_library(music_lyric_model STATIC ${MUSIC_LYRIC_GEN_SOURCES} ${MUSIC_LYRIC_SOURCES} ${MUSIC_LYRIC_PUBLIC_HEADER})
add_library(music_lyric::model ALIAS music_lyric_model)

target_compile_features(music_lyric_model PUBLIC cxx_std_20)

# MSVC reads source as the system code page (e.g. GBK) by default, but abseil/protobuf headers are UTF-8.
# /bigobj is required by protobuf gencode. Keep both PRIVATE so consumers need not depend on protobuf.
target_compile_options(music_lyric_model PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8;/bigobj>)

if(MUSIC_LYRIC_MODEL_ENABLE_JSON)
	target_compile_definitions(music_lyric_model PUBLIC MUSIC_LYRIC_MODEL_ENABLE_JSON=1)
else()
	target_compile_definitions(music_lyric_model PUBLIC MUSIC_LYRIC_MODEL_ENABLE_JSON=0)
endif()

target_include_directories(music_lyric_model
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/private"
        "${CMAKE_CURRENT_SOURCE_DIR}/private/pb"
        "${CMAKE_CURRENT_SOURCE_DIR}/private/bridge"
)

target_link_libraries(music_lyric_model PRIVATE protobuf::libprotobuf)

set(_music_lyric_ipo_ok FALSE)
if(MUSIC_LYRIC_MODEL_ENABLE_LTO)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT _music_lyric_ipo_ok OUTPUT _music_lyric_ipo_msg)
	if(_music_lyric_ipo_ok)
		set_property(TARGET music_lyric_model PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
		set_property(TARGET music_lyric_model PROPERTY INTERPROCEDURAL_OPTIMIZATION_DEBUG FALSE)
		set_property(TARGET music_lyric_model PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE)
	else()
		message(WARNING "MUSIC_LYRIC_MODEL_ENABLE_LTO is ON but IPO is not supported: ${_music_lyric_ipo_msg}")
	endif()
endif()

if(PROJECT_IS_TOP_LEVEL)
    enable_testing()

    file(GLOB_RECURSE MUSIC_LYRIC_TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/common/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/parsed/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/storage/*.cpp"
    )

    add_executable(music_lyric_model_test ${MUSIC_LYRIC_TEST_SOURCES})
    target_link_libraries(music_lyric_model_test PRIVATE music_lyric::model)
    target_include_directories(music_lyric_model_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test")
    target_compile_options(music_lyric_model_test PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

	if(_music_lyric_ipo_ok)
		set_property(TARGET music_lyric_model_test PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
		set_property(TARGET music_lyric_model_test PROPERTY INTERPROCEDURAL_OPTIMIZATION_DEBUG FALSE)
		set_property(TARGET music_lyric_model_test PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE)
	endif()

    add_test(NAME music_lyric_model_test COMMAND music_lyric_model_test)
endif()
