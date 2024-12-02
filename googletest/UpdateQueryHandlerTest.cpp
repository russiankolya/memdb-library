#include <gtest/gtest.h>

#include "CreateQueryHandler.h"
#include "InsertQueryHandler.h"
#include "Tokenizer.h"
#include "UpdateQueryHandler.h"

TEST(TestUpdateQueryHandler, SimpleTest) {
    const std::string create_query =
        "CREATE TABLE users (id : int32, login : String[10], is_admin : bool)";
    const std::string insert_query = "INSERT (10, \"zov\", true) TO users";
    const std::string update_query =
        "UPDATE users SET id = 10, login = login + \"_deleted\", is_admin = (id = 10) WHERE true";

    const Tokenizer create_tokenizer(std::move(create_query));
    const auto create_tokens = create_tokenizer.Tokenize();
    const Tokenizer insert_tokenizer(std::move(insert_query));
    const auto insert_tokens = insert_tokenizer.Tokenize();
    const Tokenizer update_tokenizer(std::move(update_query));
    const auto update_tokens = update_tokenizer.Tokenize();

    CreateQueryHandler create_handler(create_tokens);
    InsertQueryHandler insert_handler(insert_tokens);
    UpdateQueryHandler update_handler(update_tokens);

    std::map<std::string, std::unique_ptr<Table>> current_tables;
    create_handler.Execute(current_tables);
    insert_handler.Execute(current_tables);
    const auto& update_table = update_handler.Execute(current_tables);
}