#include "DeleteQueryHandler.h"

DeleteQueryHandler::DeleteQueryHandler(const std::vector<Token>& tokens) : QueryHandler(tokens) {}

void DeleteQueryHandler::Parse() {
    if (tokens_.size() < 4) {
        throw std::runtime_error("Query does not match expected format");
    }
    size_t current_index = 0;
    if (tokens_[current_index].GetValue() != "delete") {
        throw std::runtime_error("Query does not match expected format");
    }
    current_index++;
    if (tokens_[current_index].GetType() != Token::Type::Identifier) {
        throw std::runtime_error("Query does not match expected format");
    }
    table_name_ = tokens_[current_index].GetValue();
    current_index++;
    if (tokens_[current_index].GetValue() != "where") {
        throw std::runtime_error("Query does not match expected format");
    }
    current_index++;
    Expression expression;
    expression.Initialize(std::vector(tokens_.begin() + current_index, tokens_.end()));
    where_expression_ = std::move(expression);
}

std::unique_ptr<Table> DeleteQueryHandler::Execute(
    std::map<std::string, std::unique_ptr<Table>>& current_tables) {
    Parse();
    if (!current_tables.contains(table_name_)) {
        throw std::runtime_error("Table does not exist");
    }

    const auto& table = current_tables[table_name_];
    std::vector<Row> new_rows;
    for (auto& row : table->GetMutableRows()) {
        const auto& where_expression_result = where_expression_.Calc(row);
        if (!std::holds_alternative<bool>(where_expression_result)) {
            throw std::runtime_error("Where expression does not match expected format");
        }
        if (!std::get<bool>(where_expression_result)) {
            continue;
        }
        new_rows.push_back(std::move(row));
    }
    current_tables[table_name_]->SetRows(std::move(new_rows));
    return nullptr;
}
