#include "Tokenizer.h"

#include <unordered_set>
#include <vector>

Tokenizer::Tokenizer(std::string&& source) : source_(std::move(source)) {
    source_ += " ";
}

const std::unordered_set<std::string> keywords = {
    "create", "table",  "unique", "autoincrement", "key",       "int32", "bool",   "string",
    "bytes",  "insert", "to",     "select",        "from",      "where", "update", "set",
    "delete", "join",   "on",     "ordered",       "unordered", "index", "by"};

const std::unordered_set<std::string> logic_keywords = {"and", "or", "not"};

const std::unordered_set<std::string> bool_keywords = {"true", "false"};

const std::unordered_set<std::string> operators = {
    "=", "!=", "<", "<=", ">", ">=", "+", "-", "*", "/", "%", "|"};

const std::unordered_set delimiters = {':', ';', ',', '(', ')', '[', ']', '{', '}'};

std::vector<Token> Tokenizer::Tokenize() const {
    std::vector<Token> tokens;
    State current_state = State::Base;
    size_t current_pos = 0;
    std::string current_token;

    auto advance = [&] {
        ++current_pos;
        if (current_pos >= source_.size()) {
            current_state = State::End;
        }
    };

    auto peek = [&] {
        if (current_pos >= source_.size()) {
            throw std::out_of_range("Tokenizer is out of range");
        }
        return source_[current_pos];
    };

    while (current_state != State::End) {
        switch (current_state) {
            case State::Base: {
                if (!current_token.empty()) {
                    throw std::logic_error(
                        "Tokenize error (current_token is not empty in base state)");
                }
                if (isalpha(peek()) || peek() == '_') {
                    current_state = State::WordPrefix;
                    current_token += peek();
                    advance();
                    break;
                }
                if (isdigit(peek())) {
                    current_state = State::NumberPrefix;
                    current_token += peek();
                    advance();
                    break;
                }
                if (peek() == '"') {
                    current_state = State::StringPrefix;
                    current_token += peek();
                    advance();
                    break;
                }
                if (operators.contains(std::string(1, peek()))) {
                    current_state = State::OperatorPrefix;
                    current_token += peek();
                    advance();
                    break;
                }
                if (delimiters.contains(peek())) {
                    tokens.emplace_back(Token(Token::Type::Delimiter, std::string(1, peek())));
                    advance();
                    break;
                }
                if (isspace(peek())) {
                    advance();
                    break;
                }
                throw std::invalid_argument("Tokenize error (symbol was not recognized)");
            }
            case State::WordPrefix: {
                if (current_token.empty()) {
                    throw std::logic_error(
                        "Tokenize error (current_token is empty in prefix state)");
                }
                if (isalpha(peek()) || isdigit(peek()) || peek() == '_' || peek() == '.') {
                    current_token += peek();
                    advance();
                    break;
                }
                std::string current_token_lowercase = current_token;
                std::ranges::transform(current_token_lowercase, current_token_lowercase.begin(),
                                       tolower);
                if (keywords.contains(current_token_lowercase)) {
                    tokens.emplace_back(Token(Token::Type::Keyword, current_token_lowercase));
                    current_token.clear();
                    current_state = State::Base;
                    break;
                }
                if (logic_keywords.contains(current_token_lowercase)) {
                    tokens.emplace_back(Token(Token::Type::LogicKeyword, current_token_lowercase));
                    current_token.clear();
                    current_state = State::Base;
                    break;
                }
                if (bool_keywords.contains(current_token_lowercase)) {
                    tokens.emplace_back(Token(Token::Type::Bool, current_token_lowercase));
                    current_token.clear();
                    current_state = State::Base;
                    break;
                }
                tokens.emplace_back(Token(Token::Type::Identifier, current_token));
                current_token.clear();
                current_state = State::Base;
                break;
            }
            case State::NumberPrefix: {
                if (current_token.empty()) {
                    throw std::logic_error(
                        "Tokenize error (current_token is empty in prefix state)");
                }
                if (isdigit(peek())) {
                    current_token += peek();
                    advance();
                    break;
                }
                if (current_token == "0" && peek() == 'x') {
                    current_state = State::BytesPrefix;
                    current_token += peek();
                    advance();
                    break;
                }
                // TODO: add check for number correctness
                tokens.emplace_back(Token(Token::Type::Number, current_token));
                current_token.clear();
                current_state = State::Base;
                break;
            }
            case State::BytesPrefix: {
                if (current_token.empty()) {
                    throw std::logic_error(
                        "Tokenize error (current_token is empty in prefix state)");
                }
                if (isdigit(peek()) ||
                    (isalpha(peek()) && (tolower(peek()) >= 'a' && tolower(peek()) <= 'f'))) {
                    current_token += peek();
                    advance();
                    break;
                }
                if (current_token == "0x") {
                    throw std::invalid_argument("Tokenize error (invalid hexadecimal string)");
                }
                tokens.emplace_back(Token(Token::Type::Bytes, current_token));
                current_token.clear();
                current_state = State::Base;
                break;
            }
            case State::StringPrefix: {
                if (current_token.empty()) {
                    throw std::logic_error(
                        "Tokenize error (current_token is empty in prefix state)");
                }
                if (peek() == '"') {
                    current_token += peek();
                    tokens.emplace_back(Token(Token::Type::String, current_token));
                    current_token.clear();
                    advance();
                    current_state = State::Base;
                    break;
                }
                current_token += peek();
                advance();
                break;
            }
            case State::OperatorPrefix: {
                if (current_token.empty()) {
                    throw std::logic_error(
                        "Tokenize error (current_token is empty in prefix state)");
                }
                if (peek() == '=' &&
                    (current_token == "<" || current_token == ">" || current_token == "!")) {
                    current_token += peek();
                    tokens.emplace_back(Token(Token::Type::Operator, current_token));
                    current_token.clear();
                    current_state = State::Base;
                    advance();
                    break;
                }
                tokens.emplace_back(Token(Token::Type::Operator, current_token));
                current_token.clear();
                current_state = State::Base;
                break;
            }
            default: {
                throw std::logic_error("Tokenize error (unknown state)");
            }
        }
    }
    return tokens;
}
