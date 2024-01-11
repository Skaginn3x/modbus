cmake_minimum_required(VERSION 3.28)
project(modbus
  VERSION
    0.0.2
  DESCRIPTION
    "Boost asio based modbus library"
  LANGUAGES
    CXX
)

set(CMAKE_CXX_STANDARD 23)

find_package(asio REQUIRED)

set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
include(CTest)

include(FeatureSummary)

option(BUILD_TESTS "Indicates whether tests should be built." ON)
add_feature_info("BUILD_TESTS" BUILD_TESTS "Indicates whether tests should be built.")

option(BUILD_EXAMPLES "Indicates whether examples should be built." ON)
add_feature_info("BUILD_EXAMPLES" BUILD_EXAMPLES "Indicates whether examples should be built.")

add_library(modbus)
target_sources(modbus
        PUBLIC FILE_SET CXX_MODULES FILES
        src/modbus.cxx
        src/function.cxx
        src/client.cxx
        src/packet.cxx
        src/error.cxx
        src/deserialize.cxx
        src/serialize.cxx
        src/tcp.cxx
        src/constants.cxx
        src/server.cxx
        src/default_handler.cxx
)

target_link_libraries(modbus PUBLIC asio)
target_include_directories(modbus PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

# Clang format all files
file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp include/**/*.hpp examples/*.cpp tests/*.cpp src/*.cxx)
add_custom_target(
        clangformat-fix
        COMMAND clang-format
        --style=file
        -i
        ${ALL_SOURCE_FILES}
)
add_custom_target(
        clangformat-dry
        COMMAND clang-format
        --style=file
        -i
        --dry-run
        ${ALL_SOURCE_FILES}
)
