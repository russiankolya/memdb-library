#include "CreateQueryHandler.h"

#include <sstream>
#include <unordered_set>

CreateQueryHandler::CreateQueryHandler(const std::vector<Token>& tokens) : QueryHandler(tokens) {
}

Column::Attribute GetAttributeByString(const std::string& name) {
    if (name == "unique") {
        return Column::Attribute::Unique;
    }
    if (name == "autoincrement") {
        return Column::Attribute::Autoincrement;
    }
    if (name == "key") {
        return Column::Attribute::Key;
    }
    throw std::runtime_error("Unknown attribute name");
}

Cell::Type GetTypeByString(const std::string& name) {
    if (name == "int32") {
        return Cell::Type::Int32Type;
    }
    if (name == "bool") {
        return Cell::Type::BoolType;
    }
    if (name == "string") {
        return Cell::Type::StringType;
    }
    if (name == "bytes") {
        return Cell::Type::BytesType;
    }
    throw std::runtime_error("Unknown type name");
}

std::unique_ptr<Cell> GetDefaultValueByString(
    const std::pair<Cell::Type, std::optional<size_t>>& type, const std::string& value) {
    switch (type.first) {
        case Cell::Type::Int32Type: {
            return std::make_unique<CellInt32>(std::stoi(value));
        }
        case Cell::Type::BoolType: {
            if (value == "true") {
                return std::make_unique<CellBool>(true);
            }
            if (value == "false") {
                return std::make_unique<CellBool>(false);
            }
            throw std::runtime_error("Unknown value for bool type");
        }
        case Cell::Type::StringType: {
            if (value.size() > type.second) {
                throw std::runtime_error("Invalid value for string type");
            }
            return std::make_unique<CellString>(value);
        }
        case Cell::Type::BytesType: {
            const std::vector<uint8_t>& bytes = QueryHandler::HexStringToVector(value);
            if (bytes.size() > type.second) {
                throw std::runtime_error("Invalid value for bytes type");
            }
            return std::make_unique<CellBytes>(bytes);
        }
        default:
            throw std::runtime_error("Unknown column type");
    }
}

void CreateQueryHandler::Parse() {
    if (tokens_.size() < 5 || tokens_[0].GetValue() != "create" ||
        tokens_[1].GetValue() != "table" || tokens_[3].GetValue() != "(" ||
        tokens_.back().GetValue() != ")") {
        throw std::runtime_error("Query does not match expected format");
    }
    if (tokens_[2].GetType() != Token::Type::Identifier) {
        throw std::runtime_error("Incorrect name for the table");
    }
    table_name_ = tokens_[2].GetValue();

    size_t current_pos = 4;
    const size_t end_pos = tokens_.size();
    auto current_state = State::Base;

    auto advance = [&] { ++current_pos; };

    auto peek = [&] {
        if (current_pos >= end_pos) {
            throw std::out_of_range("CreateQueryHandler is out of range");
        }
        return tokens_[current_pos];
    };

    while (current_state != State::End) {
        switch (current_state) {
            case State::Base: {
                if (peek().GetValue() == ")") {
                    current_state = State::End;
                    break;
                }
                scheme_.emplace_back();
                if (peek().GetValue() == "{") {
                    current_state = State::WaitForAttribute;
                    advance();
                    break;
                }
                if (peek().GetType() == Token::Type::Identifier) {
                    current_state = State::WaitForColumnName;
                    break;
                }
                throw std::runtime_error("Query does not match expected format");
            }
            case State::WaitForAttribute: {
                if (peek().GetValue() == "}") {
                    current_state = State::WaitForColumnName;
                    advance();
                    break;
                }
                if (peek().GetType() != Token::Type::Keyword) {
                    throw std::runtime_error("Query does not match expected format");
                }
                scheme_.back().AddAttribute(GetAttributeByString(peek().GetValue()));
                advance();
                if (peek().GetValue() == "}") {
                    current_state = State::WaitForColumnName;
                    advance();
                    break;
                }
                if (peek().GetValue() == ",") {
                    advance();
                    break;
                }
                throw std::runtime_error("Query does not match expected format");
            }
            case State::WaitForColumnName: {
                if (peek().GetType() != Token::Type::Identifier) {
                    throw std::runtime_error("Query does not match expected format");
                }
                scheme_.back().SetName(std::move(peek().GetValue()));
                advance();
                if (peek().GetValue() == ":") {
                    current_state = State::WaitForDataType;
                    advance();
                    break;
                }
                throw std::runtime_error("Query does not match expected format");
            }
            case State::WaitForDataType: {
                if (peek().GetType() != Token::Type::Keyword) {
                    throw std::runtime_error("Query does not match expected format");
                }
                const auto& type_name = peek().GetValue();
                advance();
                if (peek().GetValue() == "[") {
                    advance();
                    const size_t size = std::stoi(peek().GetValue());
                    scheme_.back().SetType(GetTypeByString(type_name), size);
                    advance();
                    advance();
                } else {
                    scheme_.back().SetType(GetTypeByString(type_name));
                }
                if (peek().GetValue() == "=") {
                    current_state = State::WaitForDefaultValue;
                    advance();
                    break;
                }
                if (peek().GetValue() == ",") {
                    current_state = State::Base;
                    advance();
                    break;
                }
                if (peek().GetValue() == ")") {
                    current_state = State::End;
                    break;
                }
                throw std::runtime_error("Query does not match expected format");
            }
            case State::WaitForDefaultValue: {
                const auto& column_type = scheme_.back().GetType().first;
                if (peek().GetType() != Token::Type::Number &&
                    peek().GetType() != Token::Type::Bool &&
                    peek().GetType() != Token::Type::String &&
                    peek().GetType() != Token::Type::Bytes) {
                    throw std::runtime_error("Query does not match expected format");
                }
                if ((column_type == Cell::Type::Int32Type &&
                     peek().GetType() != Token::Type::Number) ||
                    (column_type == Cell::Type::BoolType &&
                     peek().GetType() != Token::Type::Bool) ||
                    (column_type == Cell::Type::StringType &&
                     peek().GetType() != Token::Type::String) ||
                    (column_type == Cell::Type::BytesType &&
                     peek().GetType() != Token::Type::Bytes)) {
                    throw std::runtime_error("Default value and type do not match");
                }
                scheme_.back().SetDefaultValue(
                    GetDefaultValueByString(scheme_.back().GetType(), peek().GetValue()));
                advance();
                if (peek().GetValue() == ",") {
                    current_state = State::Base;
                    advance();
                    break;
                }
                if (peek().GetValue() == ")") {
                    current_state = State::End;
                    break;
                }
            }
            default:
                throw std::runtime_error("Unknown state during parsing");
        }
    }
}

Response CreateQueryHandler::Execute(
    std::map<std::string, std::unique_ptr<Table>>& current_tables) {
    Parse();

    if (current_tables.contains(table_name_)) {
        throw std::runtime_error("Table already exists");
    }
    std::unordered_set<std::string> column_names;
    for (const auto& column : scheme_) {
        const auto& column_name = column.GetName();
        if (column_names.contains(column_name)) {
            throw std::runtime_error("Multiple column names");
        }
        column_names.insert(column_name);
    }
    auto table = std::make_unique<Table>();
    table->SetScheme(std::move(scheme_));
    current_tables.emplace(std::move(table_name_), std::move(table));
    return Response();
}