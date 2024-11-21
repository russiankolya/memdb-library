#include "Token.h"

Token::Token(const Type type, const std::string& value) : type_(type), value_(value) {}

Token::Type Token::GetType() const {
    return type_;
}

const std::string& Token::GetValue() const {
    return value_;
}


