#include "Tokenizer.h"
#include <vector>
#include <unordered_set>

Tokenizer::Tokenizer(std::string&& source) : state_(State::Base), source_(std::move(source)) {
    source_ += " ";
}

char Tokenizer::Peek() const {
    if (current_pos_ >= source_.size()) {
        throw std::out_of_range("Tokenizer is out of range");
    }
    return source_[current_pos_];
}

void Tokenizer::Advance() {
    ++current_pos_;
    if (current_pos_ >= source_.size()) {
        state_ = State::End;
    }
}

const std::unordered_set<std::string> keywords = {
     "create", "table", "unique", "autoincrement", "key", "int32", "bool", "string",
     "bytes", "insert", "to", "select", "from", "where", "update", "set",
     "delete", "join", "on", "index", "by"};

const std::unordered_set<std::string> logic_keywords = {"and", "or", "not"};

const std::unordered_set<std::string> bool_keywords = {"true", "false"};

const std::unordered_set<std::string> operators = {"=", "!=", "<", "<=", ">", ">=",
    "+" , "-", "*", "/", "%"};

const std::unordered_set delimiters = {':', ';', ',', '(', ')', '[', ']', '{', '}'};

std::vector<Token> Tokenizer::Tokenize() {
    std::vector<Token> tokens;
    while (state_ != State::End) {
        switch (state_) {
            case State::Base: {
                if (!current_token_.empty()) {
                    throw std::logic_error("Tokenize error (current_token is not empty in base state)");
                }
                if (isalpha(Peek()) || Peek() == '_') {
                    state_ = State::WordPrefix;
                    current_token_ += Peek();
                    Advance();
                    break;
                }
                if (isdigit(Peek())) {
                    state_ = State::NumberPrefix;
                    current_token_ += Peek();
                    Advance();
                    break;
                }
                if (Peek() == '"') {
                    state_ = State::StringPrefix;
                    current_token_ += Peek();
                    Advance();
                    break;
                }
                if (operators.contains(std::string(1, Peek()))) {
                    state_ = State::OperatorPrefix;
                    current_token_ += Peek();
                    Advance();
                    break;
                }
                if (delimiters.contains(Peek())) {
                    tokens.emplace_back(Token(Token::Type::Delimiter, std::string(1, Peek())));
                    Advance();
                    break;
                }
                if (Peek() == ' ') {
                    Advance();
                    break;
                }
                throw std::invalid_argument("Tokenize error (symbol was not recognized)");
            }
            case State::WordPrefix: {
                if (current_token_.empty()) {
                    throw std::logic_error("Tokenize error (current_token is empty in prefix state)");
                }
                if (isalpha(Peek()) || isdigit(Peek()) || Peek() == '_') {
                    current_token_ += Peek();
                    Advance();
                    break;
                }
                std::string current_token_lowercase = current_token_;
                std::ranges::transform(current_token_lowercase, current_token_lowercase.begin(),
                                       tolower);
                if (keywords.contains(current_token_lowercase)) {
                    tokens.emplace_back(Token(Token::Type::Keyword, current_token_lowercase));
                    current_token_.clear();
                    state_ = State::Base;
                    break;
                }
                if (logic_keywords.contains(current_token_lowercase)) {
                    tokens.emplace_back(Token(Token::Type::LogicKeyword, current_token_lowercase));
                    current_token_.clear();
                    state_ = State::Base;
                    break;
                }
                if (bool_keywords.contains(current_token_lowercase)) {
                    tokens.emplace_back(Token(Token::Type::Bool, current_token_lowercase));
                    current_token_.clear();
                    state_ = State::Base;
                    break;
                }
                tokens.emplace_back(Token(Token::Type::Identifier, current_token_));
                current_token_.clear();
                state_ = State::Base;
                break;
            }
            case State::NumberPrefix: {
                if (current_token_.empty()) {
                    throw std::logic_error("Tokenize error (current_token is empty in prefix state)");
                }
                if (isdigit(Peek())) {
                    current_token_ += Peek();
                    Advance();
                    break;
                }
                if (current_token_ == "0" && Peek() == 'x') {
                    state_ = State::BytesPrefix;
                    current_token_ += Peek();
                    Advance();
                    break;
                }
                // TODO: add check for number correctness
                tokens.emplace_back(Token(Token::Type::Number, current_token_));
                current_token_.clear();
                state_ = State::Base;
                break;
            }
            case State::BytesPrefix: {
                if (current_token_.empty()) {
                    throw std::logic_error("Tokenize error (current_token is empty in prefix state)");
                }
                if (isdigit(Peek()) || (isalpha(Peek()) &&
                    (tolower(Peek()) >= 'a' && tolower(Peek()) <= 'f'))) {
                    current_token_ += Peek();
                    Advance();
                    break;
                }
                if (current_token_ == "0x") {
                    throw std::invalid_argument("Tokenize error (invalid hexadecimal string)");
                }
                tokens.emplace_back(Token(Token::Type::Bytes, current_token_));
                current_token_.clear();
                state_ = State::Base;
                break;
            }
            case State::StringPrefix: {
                if (current_token_.empty()) {
                    throw std::logic_error("Tokenize error (current_token is empty in prefix state)");
                }
                if (Peek() == '"') {
                    current_token_ += Peek();
                    tokens.emplace_back(Token(Token::Type::String, current_token_));
                    current_token_.clear();
                    Advance();
                    state_ = State::Base;
                    break;
                }
                current_token_ += Peek();
                Advance();
                break;
            }
            case State::OperatorPrefix: {
                if (current_token_.empty()) {
                    throw std::logic_error(
                        "Tokenize error (current_token is empty in prefix state)");
                }
                if (Peek() == '=' &&
                    (current_token_ == "<" || current_token_ == ">" || current_token_ == "!")) {
                    current_token_ += Peek();
                    tokens.emplace_back(Token(Token::Type::Operator, current_token_));
                    current_token_.clear();
                    state_ = State::Base;
                    Advance();
                    break;
                }
                tokens.emplace_back(Token(Token::Type::Operator, current_token_));
                current_token_.clear();
                state_ = State::Base;
                break;
            }
            default: {
                throw std::logic_error("Tokenize error (unknown state)");
            }
        }
    }
    return tokens;
 }

