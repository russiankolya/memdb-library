#include <gtest/gtest.h>

#include <map>

#include "CreateQueryHandler.h"
#include "Tokenizer.h"

TEST(CreateQueryHandlerTest, AllTypes) {
    const std::string query = "create table t (a : int32, b: bool, c :string[10], d:bytes[1])";

    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();

    CreateQueryHandler handler(tokens);
    std::map<std::string, std::unique_ptr<Table>> current_tables;
    handler.Execute(current_tables);

    ASSERT_TRUE(current_tables.contains("t"));
    const auto& table = current_tables.at("t");

    ASSERT_EQ(table->GetScheme()[0].GetName(), "a");
    ASSERT_EQ(table->GetScheme()[0].GetType().first, Cell::Type::Int32Type);
    ASSERT_EQ(table->GetScheme()[0].GetType().second, std::nullopt);

    ASSERT_EQ(table->GetScheme()[1].GetName(), "b");
    ASSERT_EQ(table->GetScheme()[1].GetType().first, Cell::Type::BoolType);
    ASSERT_EQ(table->GetScheme()[1].GetType().second, std::nullopt);

    ASSERT_EQ(table->GetScheme()[2].GetName(), "c");
    ASSERT_EQ(table->GetScheme()[2].GetType().first, Cell::Type::StringType);
    ASSERT_EQ(table->GetScheme()[2].GetType().second, 10);

    ASSERT_EQ(table->GetScheme()[3].GetName(), "d");
    ASSERT_EQ(table->GetScheme()[3].GetType().first, Cell::Type::BytesType);
    ASSERT_EQ(table->GetScheme()[3].GetType().second, 1);
}

TEST(CreateQueryHandlerTest, Attributes) {
    const std::string query =
        "create table t ({key, autoincrement, unique} a : int32, {autoincrement} b : bool)";

    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();

    CreateQueryHandler handler(tokens);
    std::map<std::string, std::unique_ptr<Table>> current_tables;
    handler.Execute(current_tables);

    ASSERT_TRUE(current_tables.contains("t"));
    const auto& table = current_tables.at("t");

    ASSERT_EQ(table->GetScheme()[0].GetAttributes().size(), 3);

    ASSERT_EQ(table->GetScheme()[1].GetAttributes().size(), 1);
    ASSERT_TRUE(table->GetScheme()[1].GetAttributes().contains(Column::Attribute::Autoincrement));
}

TEST(CreateQueryHandlerTest, DefaultValues) {
    const std::string query =
        "create table t (a : int32 = 52, b : bool = false, c : string[3] = \"52\", d : bytes[1] = "
        "0x52)";

    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();

    CreateQueryHandler handler(tokens);
    std::map<std::string, std::unique_ptr<Table>> current_tables;
    handler.Execute(current_tables);

    ASSERT_TRUE(current_tables.contains("t"));
    const auto& table = current_tables.at("t");

    ASSERT_EQ(table->GetScheme().size(), 4);

    const auto& int32_value = static_cast<CellInt32&>(*table->GetScheme()[0].GetDefaultValue());
    ASSERT_EQ(int32_value.GetValue(), 52);

    const auto& bool_value = static_cast<CellBool&>(*table->GetScheme()[1].GetDefaultValue());
    ASSERT_EQ(bool_value.GetValue(), false);

    const auto& string_value = static_cast<CellString&>(*table->GetScheme()[2].GetDefaultValue());
    ASSERT_EQ(string_value.GetValue(), "52");

    const auto& bytes_value = static_cast<CellBytes&>(*table->GetScheme()[3].GetDefaultValue());
    ASSERT_EQ(bytes_value.GetValue().size(), 1);
    ASSERT_EQ(bytes_value.GetValue()[0], 82);
}

TEST(CreateQueryHandlerTest, NoAttributes) {
    const std::string query = "create table t ({} a : int32)";

    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();

    CreateQueryHandler handler(tokens);
    std::map<std::string, std::unique_ptr<Table>> current_tables;
    handler.Execute(current_tables);

    ASSERT_TRUE(current_tables.contains("t"));
    const auto& table = current_tables.at("t");

    ASSERT_EQ(table->GetScheme()[0].GetAttributes().size(), 0);
}

TEST(CreateQueryHandlerTest, Empty) {
    const std::string query = "create table t ()";

    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();

    CreateQueryHandler handler(tokens);
    std::map<std::string, std::unique_ptr<Table>> current_tables;
    handler.Execute(current_tables);

    ASSERT_TRUE(current_tables.contains("t"));
    const auto& table = current_tables.at("t");

    ASSERT_TRUE(table->GetScheme().empty());
}

TEST(CreateQueryHandlerTest, MultipleColumnNames) {
    const std::string query = "create table t (a : int32, a : bool)";

    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();

    CreateQueryHandler handler(tokens);
    std::map<std::string, std::unique_ptr<Table>> current_tables;
    ASSERT_THROW(handler.Execute(current_tables), std::runtime_error);
}