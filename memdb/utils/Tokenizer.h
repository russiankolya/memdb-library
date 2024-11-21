#pragma once
#include "Token.h"

class Tokenizer {
public:
    explicit Tokenizer(std::string&& source);

    std::vector<Token> Tokenize() const;

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

    std::string source_;
};
