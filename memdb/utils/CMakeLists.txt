add_library(utils Token.cpp Tokenizer.cpp QueryHandler.cpp CreateQueryHandler.cpp InsertQueryHandler.cpp SelectQueryHandler.cpp Expression.cpp UpdateQueryHandler.cpp DeleteQueryHandler.cpp)

target_include_directories(utils
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
