cmake_minimum_required(VERSION 3.26)
project(memdb-library)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(memdb-library main.cpp)

add_subdirectory(memdb)

target_link_libraries(memdb-library PUBLIC memdb)

target_include_directories(memdb-library PUBLIC
        "${PROJECT_BINARY_DIR}"
)
