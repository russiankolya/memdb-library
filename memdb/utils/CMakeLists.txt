add_library(utils Token.cpp Tokenizer.cpp QueryHandler.cpp CreateQueryHandler.cpp InsertQueryHandler.cpp)

target_include_directories(utils
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
