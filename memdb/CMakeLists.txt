add_library(memdb Database.cpp Table.cpp Column.cpp Row.cpp CellType.cpp)

target_include_directories(memdb
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
