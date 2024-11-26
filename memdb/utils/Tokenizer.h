#pragma once
#include "Token.h"

class Tokenizer {
public:
    explicit Tokenizer(const std::string& source);

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

    const std::string& source_;
};
