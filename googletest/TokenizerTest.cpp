#include <vector>

#include "gtest/gtest.h"
#include "Tokenizer.h"

TEST(TokenizerTest, Create) {
    std::string query =
        "cReAte table users ({key, autoincrement} id :\n\t"
        "int32, {unique} login: string[32], password_hash: bytes[8], is_admin:\n"
        "bool = FALSE)";
    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();
    ASSERT_EQ(tokens.size(), 36);
    const std::vector expected_tokens = {
        Token(Token::Type::Keyword, "create"),      Token(Token::Type::Keyword, "table"),
        Token(Token::Type::Identifier, "users"),    Token(Token::Type::Delimiter, "("),
        Token(Token::Type::Delimiter, "{"),         Token(Token::Type::Keyword, "key"),
        Token(Token::Type::Delimiter, ","),         Token(Token::Type::Keyword, "autoincrement"),
        Token(Token::Type::Delimiter, "}"),         Token(Token::Type::Identifier, "id"),
        Token(Token::Type::Delimiter, ":"),         Token(Token::Type::Keyword, "int32"),
        Token(Token::Type::Delimiter, ","),         Token(Token::Type::Delimiter, "{"),
        Token(Token::Type::Keyword, "unique"),      Token(Token::Type::Delimiter, "}"),
        Token(Token::Type::Identifier, "login"),    Token(Token::Type::Delimiter, ":"),
        Token(Token::Type::Keyword, "string"),      Token(Token::Type::Delimiter, "["),
        Token(Token::Type::Number, "32"),           Token(Token::Type::Delimiter, "]"),
        Token(Token::Type::Delimiter, ","),         Token(Token::Type::Identifier, "password_hash"),
        Token(Token::Type::Delimiter, ":"),         Token(Token::Type::Keyword, "bytes"),
        Token(Token::Type::Delimiter, "["),         Token(Token::Type::Number, "8"),
        Token(Token::Type::Delimiter, "]"),         Token(Token::Type::Delimiter, ","),
        Token(Token::Type::Identifier, "is_admin"), Token(Token::Type::Delimiter, ":"),
        Token(Token::Type::Keyword, "bool"),        Token(Token::Type::Operator, "="),
        Token(Token::Type::Bool, "false"),          Token(Token::Type::Delimiter, ")")};
    for (size_t i = 0; i < tokens.size(); ++i) {
        ASSERT_EQ(tokens[i].GetType(), expected_tokens[i].GetType());
        ASSERT_EQ(tokens[i].GetValue(), expected_tokens[i].GetValue());
    }
}

TEST(TokenizerTest, Insert) {
    std::string query = "insert (,\"vasya\", 0xdeadbeefdeadbeef) to users";
    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();
    ASSERT_EQ(tokens.size(), 9);
    const std::vector expected_tokens = {
        Token(Token::Type::Keyword, "insert"),  Token(Token::Type::Delimiter, "("),
        Token(Token::Type::Delimiter, ","),     Token(Token::Type::String, "\"vasya\""),
        Token(Token::Type::Delimiter, ","),     Token(Token::Type::Bytes, "0xdeadbeefdeadbeef"),
        Token(Token::Type::Delimiter, ")"),     Token(Token::Type::Keyword, "to"),
        Token(Token::Type::Identifier, "users")};
    for (size_t i = 0; i < tokens.size(); ++i) {
        ASSERT_EQ(tokens[i].GetType(), expected_tokens[i].GetType());
        ASSERT_EQ(tokens[i].GetValue(), expected_tokens[i].GetValue());
    }
}

TEST(TokenizerTest, Select) {
    std::string query = "select id, loGin from users where is_admin OR id < 10";
    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();
    ASSERT_EQ(tokens.size(), 12);
    const std::vector expected_tokens = {
        Token(Token::Type::Keyword, "select"),  Token(Token::Type::Identifier, "id"),
        Token(Token::Type::Delimiter, ","),     Token(Token::Type::Identifier, "loGin"),
        Token(Token::Type::Keyword, "from"),    Token(Token::Type::Identifier, "users"),
        Token(Token::Type::Keyword, "where"),   Token(Token::Type::Identifier, "is_admin"),
        Token(Token::Type::LogicKeyword, "or"), Token(Token::Type::Identifier, "id"),
        Token(Token::Type::Operator, "<"),      Token(Token::Type::Number, "10")};
    for (size_t i = 0; i < tokens.size(); ++i) {
        ASSERT_EQ(tokens[i].GetType(), expected_tokens[i].GetType());
        ASSERT_EQ(tokens[i].GetValue(), expected_tokens[i].GetValue());
    }
}

TEST(TokenizerTest, Update) {
    std::string query = "update users set is_admin = true where login = \"vasya\"";
    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();
    ASSERT_EQ(tokens.size(), 10);
    const std::vector expected_tokens = {
        Token(Token::Type::Keyword, "update"), Token(Token::Type::Identifier, "users"),
        Token(Token::Type::Keyword, "set"),    Token(Token::Type::Identifier, "is_admin"),
        Token(Token::Type::Operator, "="),     Token(Token::Type::Bool, "true"),
        Token(Token::Type::Keyword, "where"),  Token(Token::Type::Identifier, "login"),
        Token(Token::Type::Operator, "="),     Token(Token::Type::String, "\"vasya\"")};
    for (size_t i = 0; i < tokens.size(); ++i) {
        ASSERT_EQ(tokens[i].GetType(), expected_tokens[i].GetType());
        ASSERT_EQ(tokens[i].GetValue(), expected_tokens[i].GetValue());
    }
}

TEST(TokenizerTest, Delete) {
    std::string query = "delete users where |login| % 2 = 0";
    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();
    ASSERT_EQ(tokens.size(), 10);
    const std::vector expected_tokens = {
        Token(Token::Type::Keyword, "delete"),   Token(Token::Type::Identifier, "users"),
        Token(Token::Type::Keyword, "where"),    Token(Token::Type::Operator, "|"),
        Token(Token::Type::Identifier, "login"), Token(Token::Type::Operator, "|"),
        Token(Token::Type::Operator, "%"),       Token(Token::Type::Number, "2"),
        Token(Token::Type::Operator, "="),       Token(Token::Type::Number, "0")};
    for (size_t i = 0; i < tokens.size(); ++i) {
        ASSERT_EQ(tokens[i].GetType(), expected_tokens[i].GetType());
        ASSERT_EQ(tokens[i].GetValue(), expected_tokens[i].GetValue());
    }
}

TEST(TokenizerTest, SelectWithJoin) {
    std::string query =
        "select posts.id, users.login, posts.text\n"
        "from users\n"
        "join posts on users.id = posts.user_id where true";
    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();
    ASSERT_EQ(tokens.size(), 16);
    const std::vector expected_tokens = {
        Token(Token::Type::Keyword, "select"), Token(Token::Type::Identifier, "posts.id"),
        Token(Token::Type::Delimiter, ","),    Token(Token::Type::Identifier, "users.login"),
        Token(Token::Type::Delimiter, ","),    Token(Token::Type::Identifier, "posts.text"),
        Token(Token::Type::Keyword, "from"),   Token(Token::Type::Identifier, "users"),
        Token(Token::Type::Keyword, "join"),   Token(Token::Type::Identifier, "posts"),
        Token(Token::Type::Keyword, "on"),     Token(Token::Type::Identifier, "users.id"),
        Token(Token::Type::Operator, "="),     Token(Token::Type::Identifier, "posts.user_id"),
        Token(Token::Type::Keyword, "where"),  Token(Token::Type::Bool, "true")};
    for (size_t i = 0; i < tokens.size(); ++i) {
        ASSERT_EQ(tokens[i].GetType(), expected_tokens[i].GetType());
        ASSERT_EQ(tokens[i].GetValue(), expected_tokens[i].GetValue());
    }
}

TEST(TokenizerTest, CreateOrderedIndex) {
    std::string query = "create ordered index on users by login";
    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();
    ASSERT_EQ(tokens.size(), 7);
    const std::vector expected_tokens = {
        Token(Token::Type::Keyword, "create"),   Token(Token::Type::Keyword, "ordered"),
        Token(Token::Type::Keyword, "index"),    Token(Token::Type::Keyword, "on"),
        Token(Token::Type::Identifier, "users"), Token(Token::Type::Keyword, "by"),
        Token(Token::Type::Identifier, "login")};
    for (size_t i = 0; i < tokens.size(); ++i) {
        ASSERT_EQ(tokens[i].GetType(), expected_tokens[i].GetType());
        ASSERT_EQ(tokens[i].GetValue(), expected_tokens[i].GetValue());
    }
}

TEST(TokenizerTest, CreateUnorderedIndex) {
    std::string query = "create unordered index on _users by is_admin";
    const Tokenizer tokenizer(std::move(query));
    const auto tokens = tokenizer.Tokenize();
    ASSERT_EQ(tokens.size(), 7);
    const std::vector expected_tokens = {
        Token(Token::Type::Keyword, "create"),     Token(Token::Type::Keyword, "unordered"),
        Token(Token::Type::Keyword, "index"),      Token(Token::Type::Keyword, "on"),
        Token(Token::Type::Identifier, "_users"),  Token(Token::Type::Keyword, "by"),
        Token(Token::Type::Identifier, "is_admin")};
    for (size_t i = 0; i < tokens.size(); ++i) {
        ASSERT_EQ(tokens[i].GetType(), expected_tokens[i].GetType());
        ASSERT_EQ(tokens[i].GetValue(), expected_tokens[i].GetValue());
    }
}