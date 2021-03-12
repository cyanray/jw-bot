# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.1)

project (HTMLReader)

set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

option(HTMLReader_BUILD_SAMPLES "Build examples" ON)
if(HTMLReader_BUILD_SAMPLES)
    add_subdirectory(examples)
endif()

option(HTMLReader_BUILD_TEST "Build test" ON)
if(HTMLReader_BUILD_TEST)
    add_subdirectory(tests)
endif()