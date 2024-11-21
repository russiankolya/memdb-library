#include "gtest/gtest.h"
#include "Tokenizer.h"

TEST(TokenizerTest, CreateTest) {
    Tokenizer tokenizer(
        "create table users ({key, autoincrement} id :"
        "int32, {unique} login: string[32], password_hash: bytes[8], is_admin:"
        "bool = false)");
    auto tokens = tokenizer.Tokenize();
}