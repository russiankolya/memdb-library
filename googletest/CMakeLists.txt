project(googletest)

add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(googletest_run TokenizerTest.cpp)

target_link_libraries(googletest_run gtest gtest_main)
target_link_libraries(googletest_run memdb)