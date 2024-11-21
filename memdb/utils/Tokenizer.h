#pragma once
#include <string>

#include "Token.h"

class Tokenizer {
public:
    explicit Tokenizer(std::string&& source);

    std::vector<Token> Tokenize();

private:
    enum class State {
        Base,
        WordPrefix,
        NumberPrefix,
        StringPrefix,
        BytesPrefix,
        OperatorPrefix,
        End
    };

    State state_;
    std::string source_;
    size_t current_pos_ = 0;
    std::string current_token_;

    char Peek() const;

    void Advance();
};
