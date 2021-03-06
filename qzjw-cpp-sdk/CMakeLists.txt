cmake_minimum_required (VERSION 3.1)

project(qzjw-cpp-sdk)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json)

add_subdirectory(CURLWrapper)

aux_source_directory(./src SRCS)

add_library(${PROJECT_NAME} STATIC ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PRIVATE CURLWrapper)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)

# 源码以 UTF-8 编码保存，使用 MSVC 编译器需要加编译参数提供支持
if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    message(STATUS "optional: /UTF-8")   
endif(MSVC)

option(QZJW_CPP_SDK_BUILD_EXAMPLES "Build examples" ON)
if(QZJW_CPP_SDK_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(QZJW_CPP_SDK_BUILD_EXAMPLES)