#include <gtest/gtest.h>

#include "InsertQueryHandler.h"
#include "Tokenizer.h"

TEST(InsertQueryHandlerTest, Test) {
    const std::string query = "insert (name = \"vasya\", age = 123, is_admin = true,,,) to x";

    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();

    InsertQueryHandler handler(tokens);
    std::map<std::string, std::unique_ptr<Table>> current_tables;
    handler.Execute(current_tables);
}