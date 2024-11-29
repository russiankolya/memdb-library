#include "Expression.h"

#include <stack>

#include "QueryHandler.h"

size_t Expression::GetPrecedence(const Token& token) {
    const std::string& value = token.GetValue();
    if (value == "not") {
        return 0;
    }
    if (value == "*" || value == "/" || value == "%") {
        return 1;
    }
    if (value == "+" || value == "-") {
        return 2;
    }
    if (value == "<" || value == "<=" || value == ">" || value == ">=") {
        return 3;
    }
    if (value == "=" || value == "!=") {
        return 4;
    }
    if (value == "and") {
        return 5;
    }
    if (value == "or") {
        return 6;
    }
    if (value == "|" || value == "(") {
        return 7;
    }
    throw std::runtime_error("Unknown operation type: " + token.GetValue());
}

Expression::Expression(const std::vector<Token>& tokens) {
    std::stack<Token> operators;
    int pipe_count = 0;
    for (const auto& token : tokens) {
        const auto& type = token.GetType();
        const auto& value = token.GetValue();
        if (type == Token::Type::Identifier || type == Token::Type::Number ||
            type == Token::Type::Bool || type == Token::Type::String ||
            type == Token::Type::Bytes) {
            expression_.push_back(token);
            continue;
        }
        if (value == "(") {
            operators.push(token);
            continue;
        }
        if (value == ")") {
            while (!operators.empty() && operators.top().GetValue() != "(") {
                expression_.push_back(operators.top());
                operators.pop();
            }
            if (operators.empty()) {
                throw std::runtime_error("Missing closing parenthesis");
            }
            operators.pop();
            continue;
        }
        if (value == "|") {
            if (pipe_count == 0) {
                operators.push(token);
                pipe_count++;
            } else {
                while (operators.top().GetValue() != "|") {
                    expression_.push_back(operators.top());
                    operators.pop();
                }
                operators.pop();
                pipe_count--;
                expression_.push_back(token);
            }
            continue;
        }
        if (type == Token::Type::Operator || type == Token::Type::LogicKeyword) {
            while (!operators.empty() && GetPrecedence(token) > GetPrecedence(operators.top())) {
                expression_.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }
    while (!operators.empty()) {
        expression_.push_back(operators.top());
        operators.pop();
    }
}

std::variant<int32_t, bool, std::string, std::vector<uint8_t>> GetValueByToken(const Token& token) {
    switch (token.GetType()) {
        case Token::Type::Number:
            return std::stoi(token.GetValue());
        case Token::Type::Bool:
            if (token.GetValue() == "true") {
                return true;
            }
            if (token.GetValue() == "false") {
                return false;
            }
            throw std::runtime_error("Unknown bool value: " + token.GetValue());
        case Token::Type::String:
            return token.GetValue();
        case Token::Type::Bytes:
            return QueryHandler::HexStringToVector(token.GetValue());
        default:
            throw std::runtime_error("Wrong token type to extract value: " + token.GetValue());
    }
}

std::variant<int32_t, bool, std::string, std::vector<uint8_t>> Expression::Calc(
    const Row& row) const {
    std::map<std::string, std::variant<int32_t, bool, std::string, std::vector<uint8_t>>> value_by_identifier;
    for (size_t i = 0; i < row.GetScheme().size(); i++) {
        value_by_identifier[row.GetScheme()[i].GetName()] = row.GetCells()[i]->GetValue();
    }
    std::stack<std::variant<int32_t, bool, std::string, std::vector<uint8_t>>> calculation_stack;
    for (const auto& token : expression_) {
        const auto& type = token.GetType();
        const auto& value = token.GetValue();
        if (type == Token::Type::Number || type == Token::Type::Bool ||
            type == Token::Type::String || type == Token::Type::Bytes) {
            calculation_stack.push(GetValueByToken(token));
        }
        if (type == Token::Type::Identifier) {
            calculation_stack.push(value_by_identifier[value]);
        }
        if (type == Token::Type::Delimiter) {
            if (value == "|") {
                if (calculation_stack.empty()) {
                    throw std::runtime_error("Unexpected error in expression calculation");
                }
                const auto& top_value = calculation_stack.top();
                if (std::holds_alternative<std::string>(top_value)) {
                    const int value_size = std::get<std::string>(top_value).size();
                    calculation_stack.pop();
                    calculation_stack.push(value_size);
                } else if (std::holds_alternative<std::vector<uint8_t>>(top_value)) {
                    const int value_size = std::get<std::vector<uint8_t>>(top_value).size();
                    calculation_stack.pop();
                    calculation_stack.push(value_size);
                } else {
                    throw std::runtime_error("Wrong usage of length operator");
                }
            } else {
                throw std::runtime_error("Unexpected delimiter in expression");
            }
        }
        if (type == Token::Type::LogicKeyword) {
            if (calculation_stack.empty()) {
                throw std::runtime_error("Unexpected error in expression calculation");
            }
            if (!std::holds_alternative<bool>(calculation_stack.top())) {
                throw std::runtime_error("Applying logical operator to type different from bool");
            }
            if (value == "not") {
                const bool top_value = std::get<bool>(calculation_stack.top());
                calculation_stack.pop();
                calculation_stack.push(!top_value);
            } else {
                const bool top_value = std::get<bool>(calculation_stack.top());
                calculation_stack.pop();
                if (calculation_stack.empty()) {
                    throw std::runtime_error("Unexpected error in expression calculation");
                }
                if (!std::holds_alternative<bool>(calculation_stack.top())) {
                    throw std::runtime_error(
                        "Applying logical operator to type different from bool");
                }
                const bool second_top_value = std::get<bool>(calculation_stack.top());
                calculation_stack.pop();
                if (value == "and") {
                    calculation_stack.push(top_value && second_top_value);
                } else {
                    calculation_stack.push(top_value || second_top_value);
                }
            }
        }
        if (type == Token::Type::Operator) {
            const auto second_top_value = calculation_stack.top();
            calculation_stack.pop();
            if (calculation_stack.empty()) {
                throw std::runtime_error("Unexpected error in expression calculation");
            }
            const auto top_value = calculation_stack.top();
            calculation_stack.pop();
            if (top_value.index() != second_top_value.index()) {
                throw std::runtime_error(
                    "Trying to apply operator for values with different types");
            }
            if (value == "=" || value == "!=" || value == "<" || value == "<=" || value == ">" ||
                value == ">=") {
                if (value == "=") {
                    calculation_stack.push(top_value == second_top_value);
                }
                if (value == "!=") {
                    calculation_stack.push(top_value != second_top_value);
                }
                if (value == "<") {
                    calculation_stack.push(top_value < second_top_value);
                }
                if (value == "<=") {
                    calculation_stack.push(top_value <= second_top_value);
                }
                if (value == ">") {
                    calculation_stack.push(top_value > second_top_value);
                }
                if (value == ">=") {
                    calculation_stack.push(top_value >= second_top_value);
                }  // TODO: realize operator for comparing bytes
            }
            if (value == "+" && std::holds_alternative<std::string>(top_value)) {
                calculation_stack.push(std::get<std::string>(top_value) +
                                       std::get<std::string>(second_top_value));
                continue;
            }
            if (value == "+" || value == "-" || value == "*" || value == "/" || value == "%") {
                if (!std::holds_alternative<int32_t>(top_value)) {
                    throw std::runtime_error("Trying to apply arithmetic operator for non-integer value");
                }
                if (value == "+") {
                    calculation_stack.push(std::get<int32_t>(top_value) + std::get<int32_t>(second_top_value));
                }
                if (value == "-") {
                    calculation_stack.push(std::get<int32_t>(top_value) - std::get<int32_t>(second_top_value));
                }
                if (value == "*") {
                    calculation_stack.push(std::get<int32_t>(top_value) * std::get<int32_t>(second_top_value));
                }
                if (value == "/") {
                    calculation_stack.push(std::get<int32_t>(top_value) / std::get<int32_t>(second_top_value));
                }
                if (value == "%") {
                    calculation_stack.push(std::get<int32_t>(top_value) % std::get<int32_t>(second_top_value));
                }
            }
        }
    }
    if (calculation_stack.size() != 1) {
        throw std::runtime_error("Unexpected error in expression calculation");
    }
    return calculation_stack.top();
}