#include <CreateQueryHandler.h>
#include <gtest/gtest.h>

#include "InsertQueryHandler.h"
#include "Tokenizer.h"

TEST(InsertQueryHandlerTest, AllValuesEqualSign) {
    const std::string create_query = "create table t (name : string[5], age : int32, is_admin : bool)";
    const std::string insert_query = "insert (name = \"vasya\", age = 123, is_admin = true) to t";

    const Tokenizer create_tokenizer(std::move(create_query));
    const auto create_tokens = create_tokenizer.Tokenize();
    const Tokenizer insert_tokenizer(std::move(insert_query));
    const auto insert_tokens = insert_tokenizer.Tokenize();

    CreateQueryHandler create_handler(create_tokens);
    InsertQueryHandler insert_handler(insert_tokens);

    std::map<std::string, std::unique_ptr<Table>> current_tables;
    create_handler.Execute(current_tables);
    insert_handler.Execute(current_tables);

    const auto& table = current_tables.at("t");
    ASSERT_EQ(table->GetRows().size(), 1);

    const auto string_value = std::get<std::string>(table->GetRows()[0].GetCells()[0]->GetValue());
    ASSERT_EQ(string_value, "vasya");

    const auto int32_value = std::get<int32_t>(table->GetRows()[0].GetCells()[1]->GetValue());
    ASSERT_EQ(int32_value, 123);

    const auto bool_value = std::get<bool>(table->GetRows()[0].GetCells()[2]->GetValue());
    ASSERT_EQ(bool_value, true);
}

TEST(InsertQueryHandlerTest, MissingValuesEqualSign) {
    const std::string create_query = "create table t (name : string[5], age : int32 = 52, is_admin : bool = true)";
    const std::string insert_query = "insert (name = \"vasya\") to t";

    const Tokenizer create_tokenizer(std::move(create_query));
    const auto create_tokens = create_tokenizer.Tokenize();
    const Tokenizer insert_tokenizer(std::move(insert_query));
    const auto insert_tokens = insert_tokenizer.Tokenize();

    CreateQueryHandler create_handler(create_tokens);
    InsertQueryHandler insert_handler(insert_tokens);

    std::map<std::string, std::unique_ptr<Table>> current_tables;
    create_handler.Execute(current_tables);
    insert_handler.Execute(current_tables);

    const auto& table = current_tables.at("t");
    ASSERT_EQ(table->GetRows().size(), 1);

    const auto string_value = std::get<std::string>(table->GetRows()[0].GetCells()[0]->GetValue());
    ASSERT_EQ(string_value, "vasya");

    const auto int32_value = std::get<int32_t>(table->GetRows()[0].GetCells()[1]->GetValue());
    ASSERT_EQ(int32_value, 52);

    const auto bool_value = std::get<bool>(table->GetRows()[0].GetCells()[2]->GetValue());
    ASSERT_EQ(bool_value, true);
}

TEST(InsertQueryHandlerTest, AllValuesNoEqualSign) {
    const std::string create_query = "create table t (name : string[5], age : int32, is_admin : bool)";
    const std::string insert_query = "insert (\"vasya\", 123, true) to t";

    const Tokenizer create_tokenizer(std::move(create_query));
    const auto create_tokens = create_tokenizer.Tokenize();
    const Tokenizer insert_tokenizer(std::move(insert_query));
    const auto insert_tokens = insert_tokenizer.Tokenize();

    CreateQueryHandler create_handler(create_tokens);
    InsertQueryHandler insert_handler(insert_tokens);

    std::map<std::string, std::unique_ptr<Table>> current_tables;
    create_handler.Execute(current_tables);
    insert_handler.Execute(current_tables);

    const auto& table = current_tables.at("t");
    ASSERT_EQ(table->GetRows().size(), 1);

    const auto string_value = std::get<std::string>(table->GetRows()[0].GetCells()[0]->GetValue());
    ASSERT_EQ(string_value, "vasya");

    const auto int32_value = std::get<int32_t>(table->GetRows()[0].GetCells()[1]->GetValue());
    ASSERT_EQ(int32_value, 123);

    const auto bool_value = std::get<bool>(table->GetRows()[0].GetCells()[2]->GetValue());
    ASSERT_EQ(bool_value, true);
}

TEST(InsertQueryHandlerTest, MissingValuesNoEqualSign) {
    const std::string create_query = "create table t (name : string[5], age : int32 = 52, is_admin : bool = true)";
    const std::string insert_query = "insert (\"vasya\",,) to t";

    const Tokenizer create_tokenizer(std::move(create_query));
    const auto create_tokens = create_tokenizer.Tokenize();
    const Tokenizer insert_tokenizer(std::move(insert_query));
    const auto insert_tokens = insert_tokenizer.Tokenize();

    CreateQueryHandler create_handler(create_tokens);
    InsertQueryHandler insert_handler(insert_tokens);

    std::map<std::string, std::unique_ptr<Table>> current_tables;
    create_handler.Execute(current_tables);
    insert_handler.Execute(current_tables);

    const auto& table = current_tables.at("t");
    ASSERT_EQ(table->GetRows().size(), 1);

    const auto string_value = std::get<std::string>(table->GetRows()[0].GetCells()[0]->GetValue());
    ASSERT_EQ(string_value, "vasya");

    const auto int32_value = std::get<int32_t>(table->GetRows()[0].GetCells()[1]->GetValue());
    ASSERT_EQ(int32_value, 52);

    const auto bool_value = std::get<bool>(table->GetRows()[0].GetCells()[2]->GetValue());
    ASSERT_EQ(bool_value, true);
}