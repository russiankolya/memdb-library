add_library(memdb Database.cpp)

target_include_directories(memdb
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
