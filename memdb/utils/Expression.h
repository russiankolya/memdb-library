#pragma once
#include <variant>
#include <vector>

#include "Row.h"
#include "Token.h"

class Expression {
public:
    void Initialize(const std::vector<Token>& tokens);

    std::variant<int32_t, bool, std::string, std::vector<uint8_t>> Calc(const Row& row) const;

    static size_t GetPrecedence(const Token& token);
private:
    std::vector<Token> expression_;
};
