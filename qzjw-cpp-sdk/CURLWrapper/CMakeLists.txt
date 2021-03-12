cmake_minimum_required (VERSION 3.1)

project (CURLWrapper)

set(CMAKE_CXX_STANDARD 11)

find_package(CURL)


add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} INTERFACE CURL::libcurl)

option(CURLWrapper_BUILD_SAMPLES "Build examples" OFF)
if(CURLWrapper_BUILD_SAMPLES)
    add_subdirectory(examples)
endif()
