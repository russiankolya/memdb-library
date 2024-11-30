#include "InsertQueryHandler.h"

#include <map>
#include <sstream>

InsertQueryHandler::InsertQueryHandler(const std::vector<Token>& tokens) : QueryHandler(tokens) {
}

std::unique_ptr<Cell> GetValuePointerByToken(const Token& token) {
    switch (token.GetType()) {
        case Token::Type::Number: {
            return std::make_unique<CellInt32>(std::stoi(token.GetValue()));
        }
        case Token::Type::Bool: {
            if (token.GetValue() == "true") {
                return std::make_unique<CellBool>(true);
            }
            if (token.GetValue() == "false") {
                return std::make_unique<CellBool>(false);
            }
            throw std::invalid_argument("Invalid value for bool token");
        }
        case Token::Type::String: {
            return std::make_unique<CellString>(token.GetValue());
        }
        case Token::Type::Bytes: {
            return std::make_unique<CellBytes>(QueryHandler::HexStringToVector(token.GetValue()));
        }
        default:
            throw std::invalid_argument("Query does not match expected format");
    }
}

void InsertQueryHandler::Parse() {
    if (tokens_.size() < 5 || tokens_[0].GetValue() != "insert" || tokens_[1].GetValue() != "(" ||
        tokens_[tokens_.size() - 3].GetValue() != ")" ||
        tokens_[tokens_.size() - 2].GetValue() != "to") {
        throw std::runtime_error("Query does not match expected format");
    }
    if (tokens_[tokens_.size() - 1].GetType() != Token::Type::Identifier) {
        throw std::runtime_error("Incorrect name for the table");
    }
    table_name_ = tokens_[tokens_.size() - 1].GetValue();

    bool is_equal_sign = false;
    for (size_t i = 2; i < tokens_.size() - 3; ++i) {
        if (tokens_[i].GetValue() == "=") {
            is_equal_sign = true;
        }
    }

    if (is_equal_sign) {
        std::map<std::string, std::unique_ptr<Cell>> value_by_column_name;
        for (size_t i = 2; i < tokens_.size() - 3; ++i) {
            if (tokens_[i].GetValue() == ",") {
                continue;
            }
            if (tokens_[i].GetType() != Token::Type::Identifier) {
                throw std::runtime_error("Incorrect name for column");
            }
            if (tokens_[i + 1].GetValue() != "=") {
                throw std::runtime_error("Query does not match expected format");
            }
            if (tokens_[i + 3].GetValue() != "," && tokens_[i + 3].GetValue() != ")") {
                throw std::runtime_error("Query does not match expected format");
            }
            value_by_column_name[tokens_[i].GetValue()] = GetValuePointerByToken(tokens_[i + 2]);
            i = i + 3;
        }
        parsed_row_ = std::move(value_by_column_name);
    } else {
        std::vector<std::unique_ptr<Cell>> values;
        for (size_t i = 2; i < tokens_.size() - 3; ++i) {
            if (tokens_[i].GetValue() == ",") {
                values.push_back(nullptr);
            } else {
                values.push_back(GetValuePointerByToken(tokens_[i]));
                ++i;
            }
        }
        if (tokens_[tokens_.size() - 4].GetValue() == ",") {
            values.push_back(nullptr);
        }
        parsed_row_ = std::move(values);
    }
}

std::unique_ptr<Table> InsertQueryHandler::Execute(
    std::map<std::string, std::unique_ptr<Table>>& current_tables) {
    Parse();

    if (!current_tables.contains(table_name_)) {
        throw std::runtime_error("Table not found");
    }

    const auto& table = current_tables[table_name_];
    const auto& scheme = table->GetScheme();

    if (std::holds_alternative<std::vector<std::unique_ptr<Cell>>>(parsed_row_)) {
        const auto& parsed_row = std::get<std::vector<std::unique_ptr<Cell>>>(parsed_row_);
        if (scheme.size() != parsed_row.size()) {
            throw std::runtime_error("Values do not match scheme");
        }
        std::vector<std::unique_ptr<Cell>> cells(scheme.size());
        for (size_t i = 0; i < scheme.size(); ++i) {
            if (parsed_row[i] == nullptr) {
                if (scheme[i].GetDefaultValue() == nullptr) {
                    throw std::runtime_error("No default for missing value");
                }
                cells[i] = scheme[i].GetDefaultValue()->Clone();
                continue;
            }
            if (parsed_row[i]->GetType() != scheme[i].GetType().first) {
                throw std::runtime_error("Values do not match scheme");
            }
            if (scheme[i].GetType().first == Cell::Type::StringType ||
                scheme[i].GetType().first == Cell::Type::BytesType) {
                if (parsed_row[i]->GetSize() > scheme[i].GetType().second) {
                    throw std::runtime_error("Values do not match scheme");
                }
            }
            cells[i] = std::move(std::get<std::vector<std::unique_ptr<Cell>>>(parsed_row_)[i]);
        }
        Row row(scheme);
        row.SetCells(std::move(cells));
        table->AddRow(std::move(row));
    } else {
        const auto& parsed_map =
            std::get<std::map<std::string, std::unique_ptr<Cell>>>(parsed_row_);
        std::set<std::string> scheme_column_names;
        for (const auto& column : scheme) {
            if (parsed_map.contains(column.GetName())) {
                if (parsed_map.at(column.GetName())->GetSize() > column.GetType().second) {
                    throw std::runtime_error("Values do not match scheme");
                }
            } else {
                if (column.GetDefaultValue() == nullptr) {
                    throw std::runtime_error("No default for missing value");
                }
            }
            scheme_column_names.insert(column.GetName());
        }
        for (const auto& [column_name, value] : parsed_map) {
            if (!scheme_column_names.contains(column_name)) {
                throw std::runtime_error("Values do not match scheme");
            }
        }
        std::vector<std::unique_ptr<Cell>> cells(scheme.size());
        for (size_t i = 0; i < scheme.size(); ++i) {
            if (const auto& column_name = scheme[i].GetName(); parsed_map.contains(column_name)) {
                cells[i] =
                    std::move(std::get<std::map<std::string, std::unique_ptr<Cell>>>(parsed_row_)
                                  .at(column_name));
            } else {
                cells[i] = scheme[i].GetDefaultValue()->Clone();
            }
        }
        Row row(scheme);
        row.SetCells(std::move(cells));
        table->AddRow(std::move(row));
    }
    return nullptr;
}
