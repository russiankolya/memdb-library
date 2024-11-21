#pragma once
#include <string>

class Token {
public:
    enum class Type {
        Keyword,
        LogicKeyword,
        Operator,
        Delimiter,
        Identifier,
        Number,
        Bool,
        String,
        Bytes
    };

    Token(const Type type, const std::string& value) : type_(type), value_(value) {};
private:
    Type type_{};
    std::string value_;
};
