#include "UpdateQueryHandler.h"

UpdateQueryHandler::UpdateQueryHandler(const std::vector<Token>& tokens) : QueryHandler(tokens) {
}

void UpdateQueryHandler::Parse() {
    if (tokens_.size() < 8) {
        throw std::runtime_error("Query does not match expected format");
    }
    size_t current_index = 0;
    if (tokens_[current_index].GetValue() != "update") {
        throw std::runtime_error("Query does not match expected format");
    }
    current_index++;
    if (tokens_[current_index].GetType() != Token::Type::Identifier) {
        throw std::runtime_error("Query does not match expected format");
    }
    table_name_ = tokens_[current_index].GetValue();
    current_index++;
    if (tokens_[current_index].GetValue() != "set") {
        throw std::runtime_error("Query does not match expected format");
    }

    while (current_index < tokens_.size() && tokens_[current_index].GetValue() != "where") {
        current_index++;
        if (tokens_[current_index].GetType() != Token::Type::Identifier) {
            throw std::runtime_error("Incorrect name of column");
        }
        if (current_index + 1 >= tokens_.size() || tokens_[current_index + 1].GetValue() != "=") {
            throw std::runtime_error("Query does not match expected format");
        }
        const size_t begin_expression_index = current_index + 2;
        size_t end_expression_index = begin_expression_index + 1;
        while (end_expression_index < tokens_.size() &&
               tokens_[end_expression_index].GetValue() != "," &&
                tokens_[end_expression_index].GetValue() != "where") {
            end_expression_index++;
        }
        if (tokens_[end_expression_index].GetValue() != "," && tokens_[end_expression_index].GetValue() != "where") {
            throw std::runtime_error("Query does not match expected format");
        }
        Expression expression;
        expression.Initialize(std::vector(tokens_.begin() + begin_expression_index, tokens_.begin() + end_expression_index));
        assignments_[tokens_[current_index].GetValue()] = std::move(expression);
        current_index = end_expression_index;
    }
    if (current_index > tokens_.size() || tokens_[current_index].GetValue() != "where") {
        throw std::runtime_error("Query does not match expected format");
    }
    current_index++;
    Expression expression;
    expression.Initialize(std::vector(tokens_.begin() + current_index, tokens_.end()));
    where_expression_ = std::move(expression);
}

std::unique_ptr<Cell> GetCellByVariantValue(std::variant<int32_t, bool, std::string, std::vector<uint8_t>> value) {
    if (std::holds_alternative<int32_t>(value)) {
        return std::make_unique<CellInt32>(std::get<int32_t>(value));
    }
    if (std::holds_alternative<bool>(value)) {
        return std::make_unique<CellBool>(std::get<bool>(value));
    }
    if (std::holds_alternative<std::string>(value)) {
        return std::make_unique<CellString>(std::get<std::string>(value));
    }
    if (std::holds_alternative<std::vector<uint8_t>>(value)) {
        return std::make_unique<CellBytes>(std::get<std::vector<uint8_t>>(value));
    }
    throw std::runtime_error("Unknown variant value type");
}

std::unique_ptr<Table> UpdateQueryHandler::Execute(
    std::map<std::string, std::unique_ptr<Table>>& current_tables) {
    Parse();
    if (!current_tables.contains(table_name_)) {
        throw std::runtime_error("Table does not exist");
    }

    const auto& table = current_tables[table_name_];
    const auto& scheme = table->GetScheme();

    std::set<std::string> columns_in_scheme;
    for (const auto& column : scheme) {
        columns_in_scheme.insert(column.GetName());
    }

    for (const auto&[column_name, expression] : assignments_) {
        if (!columns_in_scheme.contains(column_name)) {
            throw std::runtime_error("Column does not exist");
        }
    }

    for (auto& row : table->GetMutableRows()) {
        const auto& where_expression_result = where_expression_.Calc(row);
        if (!std::holds_alternative<bool>(where_expression_result)) {
            throw std::runtime_error("Where expression does not match expected format");
        }
        if (!std::get<bool>(where_expression_result)) {
            continue;
        }
        for (size_t i = 0; i < scheme.size(); ++i) {
            if (assignments_.contains(scheme[i].GetName())) {
                const auto& old_value = row.GetCells().at(i)->GetValue();
                const auto& new_value = assignments_[scheme[i].GetName()].Calc(row);
                if (old_value.index() != new_value.index()) {
                    throw std::runtime_error("Result of set assignment does not match expected format");
                }
                row.SetCell(i, GetCellByVariantValue(std::move(new_value)));
            }
        }
    }
    return nullptr;
}

