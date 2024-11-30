#include <gtest/gtest.h>
#include <InsertQueryHandler.h>

#include "CreateQueryHandler.h"
#include "SelectQueryHandler.h"
#include "Tokenizer.h"

TEST(TestCreateQueryHandler, SimpleTest) {
    const std::string create_query = "CREATE TABLE users (id : int32, login : String[10], is_admin : bool)";
    const std::string insert_query = "INSERT (10, \"zov\", true) TO users";
    const std::string select_query = "SELECT * FROM users WHERE is_admin OR id < 10";

    const Tokenizer create_tokenizer(std::move(create_query));
    const auto create_tokens = create_tokenizer.Tokenize();
    const Tokenizer insert_tokenizer(std::move(insert_query));
    const auto insert_tokens = insert_tokenizer.Tokenize();
    const Tokenizer select_tokenizer(std::move(select_query));
    const auto select_tokens = select_tokenizer.Tokenize();

    CreateQueryHandler create_handler(create_tokens);
    InsertQueryHandler insert_handler(insert_tokens);
    SelectQueryHandler select_handler(select_tokens);

    std::map<std::string, std::unique_ptr<Table>> current_tables;
    create_handler.Execute(current_tables);
    insert_handler.Execute(current_tables);
    const auto& select_table = select_handler.Execute(current_tables);
    1;
}