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

    Token(Type type, const std::string& value);

    Type GetType() const;
    const std::string& GetValue() const;
private:
    Type type_{};
    std::string value_;
};
