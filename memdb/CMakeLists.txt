add_library(memdb Database.cpp Table.cpp Column.cpp Row.cpp DataType.cpp Response.cpp
        utils/QueryHandler.cpp
        utils/Token.cpp
        utils/Tokenizer.cpp)

target_include_directories(memdb
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)
