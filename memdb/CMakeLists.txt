add_library(memdb Database.cpp Table.cpp Column.cpp Row.cpp Cell.cpp Response.cpp)

add_subdirectory(utils)

target_include_directories(memdb
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/utils>
)

target_link_libraries(memdb PUBLIC utils)
