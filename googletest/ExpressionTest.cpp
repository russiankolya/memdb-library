#include <InsertQueryHandler.h>

#include "CreateQueryHandler.h"
#include "Expression.h"
#include "gtest/gtest.h"
#include "Tokenizer.h"

TEST(TestExpression, ExpressionNoVariables) {
    const std::string expression = "((1 + 2) * 3) <= (|\"abcdef\" + \"abc\"|)";
    const Tokenizer tokenizer(expression);
    const auto& tokens = tokenizer.Tokenize();
    Expression expr;
    expr.Initialize(tokens);
    const auto res = expr.Calc(Row({}));
    ASSERT_EQ(std::get<bool>(res), true);
}

TEST(TestExpression, ExpressionNoVariablesWithLogicalOperatorAnd) {
    const std::string expression = "(((1 + 2) * 3) <= (|\"abcdef\" + \"abc\"|)) AND (1 < 0)";
    const Tokenizer tokenizer(expression);
    const auto& tokens = tokenizer.Tokenize();
    Expression expr;
    expr.Initialize(tokens);
    const auto res = expr.Calc(Row({}));
    ASSERT_EQ(std::get<bool>(res), false);
}

TEST(TestExpression, ExpressionNoVariablesWithLogicalOperatorNot) {
    const std::string expression = "(((1 + 2) * 3) <= (|\"abcdef\" + \"abc\"|)) AND NOT (1 < 0)";
    const Tokenizer tokenizer(expression);
    const auto& tokens = tokenizer.Tokenize();
    Expression expr;
    expr.Initialize(tokens);
    const auto res = expr.Calc(Row({}));
    ASSERT_EQ(std::get<bool>(res), true);
}

TEST(TestExpression, ExpressionNoVariablesWithBytes) {
    const std::string expression = "(|0xaabbccddeeff| * 2 <= 12) OR false";
    const Tokenizer tokenizer(expression);
    const auto& tokens = tokenizer.Tokenize();
    Expression expr;
    expr.Initialize(tokens);
    const auto res = expr.Calc(Row({}));
    ASSERT_EQ(std::get<bool>(res), true);
}

TEST(TestExpression, ExpressionWithVariable) {
    const std::string create_query = "CREATE TABLE t (x : int32 = 3)";
    const Tokenizer create_tokenizer(create_query);
    const auto& create_tokens = create_tokenizer.Tokenize();
    CreateQueryHandler create_handler(create_tokens);

    const std::string insert_query = "INSERT (x = 3) TO t";
    const Tokenizer insert_tokenizer(insert_query);
    const auto& insert_tokens = insert_tokenizer.Tokenize();
    InsertQueryHandler insert_handler(insert_tokens);

    std::map<std::string, std::unique_ptr<Table>> tables;
    create_handler.Execute(tables);
    insert_handler.Execute(tables);

    const std::string expression = "(x + 1) % 5 = 4";
    const Tokenizer expression_tokenizer(expression);
    const auto& expression_tokens = expression_tokenizer.Tokenize();
    Expression expr;
    expr.Initialize(expression_tokens);
    const auto res = expr.Calc(tables["t"]->GetRows()[0]);
    ASSERT_EQ(std::get<bool>(res), true);
}

TEST(TestExpression, ExpressionWithDifferentTypes) {
    const std::string expression = "true + 1";
    const Tokenizer tokenizer(expression);
    const auto& tokens = tokenizer.Tokenize();
    Expression expr;
    expr.Initialize(tokens);
    ASSERT_THROW(expr.Calc(Row({})), std::runtime_error);
}

TEST(TestExpression, ExpressionWithMissingOpeningParanthesis) {
    const std::string expression = "(1 + 2) + 3)";
    const Tokenizer tokenizer(expression);
    const auto& tokens = tokenizer.Tokenize();
    Expression expr;
    ASSERT_THROW(expr.Initialize(tokens), std::runtime_error);
}

TEST(TestExpression, ExpressionWithMissingClosingParanthesis) {
    const std::string expression = "((1 + 2) + 3";
    const Tokenizer tokenizer(expression);
    const auto& tokens = tokenizer.Tokenize();
    Expression expr;
    ASSERT_THROW(expr.Initialize(tokens), std::runtime_error);
}
