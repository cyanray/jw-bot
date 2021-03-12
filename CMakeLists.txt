cmake_minimum_required (VERSION 3.1)

project(jw-bot)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-sqlite3)
find_package(SQLiteCpp)

# 不要构建 mirai-cpp 自带的 examples
set(MIRAI_CPP_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
add_subdirectory(mirai-cpp)

# 不要构建 qzjw-cpp-sdk 自带的 examples
set(QZJW_CPP_SDK_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
add_subdirectory(qzjw-cpp-sdk)

# 不要构建 HTMLReader 自带的 examples 和 test
set(HTMLReader_BUILD_SAMPLES OFF CACHE BOOL "Build examples" FORCE)
set(HTMLReader_BUILD_TEST OFF CACHE BOOL "Build test" FORCE)
add_subdirectory(HTMLReader)

# 源码以 UTF-8 编码保存，使用 MSVC 编译器需要加编译参数提供支持
if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    message(STATUS "optional: /UTF-8")   
endif(MSVC)

aux_source_directory(./src SRCS)
add_executable(${PROJECT_NAME} ${SRCS})
include_directories(include)
target_link_libraries(${PROJECT_NAME} PRIVATE qzjw-cpp-sdk)
target_link_libraries(${PROJECT_NAME} PRIVATE mirai-cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(${PROJECT_NAME} PRIVATE SQLiteCpp)
target_link_libraries(${PROJECT_NAME} PRIVATE HTMLReader)
target_compile_definitions(${PROJECT_NAME} PRIVATE MIRAICPP_STATICLIB)

if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    target_link_libraries(${PROJECT_NAME} PRIVATE -lstdc++fs)
    target_link_libraries(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
endif()