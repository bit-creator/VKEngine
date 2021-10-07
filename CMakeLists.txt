cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_COMPILER clang++-13)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/.build)
set(PRECOMPILED_MODULES_DIR ${BUILD_DIR}/precompiled)
set(3RDPARTY_DIR ${CMAKE_SOURCE_DIR}/3rdparty)
set(CMAKE_SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/cmakeScripts)

project(VKEngine)

add_subdirectory(3rdparty)
add_subdirectory(src)