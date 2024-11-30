#include "SelectQueryHandler.h"

SelectQueryHandler::SelectQueryHandler(const std::vector<Token>& tokens) : QueryHandler(tokens) {
}

void SelectQueryHandler::Parse() {
    if (tokens_.size() < 6) {
        throw std::runtime_error("Query does not match expected format");
    }
    size_t current_index = 0;
    if (tokens_[current_index].GetValue() != "select") {
        throw std::runtime_error("Query does not match expected format");
    }
    current_index++;
    if (tokens_[current_index].GetType() == Token::Type::Identifier) {
        column_names_.emplace();
        while (current_index + 1 < tokens_.size() && tokens_[current_index + 1].GetValue() == ",") {
            column_names_->push_back(tokens_[current_index].GetValue());
            current_index += 2;
        }
        column_names_->push_back(tokens_[current_index].GetValue());
    } else {
        if (tokens_[current_index].GetValue() != "*") {
            throw std::runtime_error("Query does not match expected format");
        }
    }
    current_index++;
    if (tokens_[current_index].GetValue() != "from") {
        throw std::runtime_error("Query does not match expected format");
    }
    current_index++;
    if (tokens_[current_index].GetType() != Token::Type::Identifier) {
        throw std::runtime_error("Incorrect table name");
    }
    table_name_ = tokens_[current_index].GetValue();
    current_index++;
    if (tokens_[current_index].GetValue() != "where") {
        throw std::runtime_error("Query does not match expected format");
    }
    current_index++;
    where_expression_.Initialize(
        std::vector<Token>(tokens_.begin() + current_index, tokens_.end()));
}

std::unique_ptr<Table> SelectQueryHandler::Execute(
    std::map<std::string, std::unique_ptr<Table>>& current_tables) {
    Parse();
    if (!current_tables.contains(table_name_)) {
        throw std::runtime_error("Table does not exist");
    }

    const auto& table = current_tables[table_name_];
    const auto& scheme = table->GetScheme();
    std::map<std::string, size_t> column_index;
    for (size_t i = 0; i < scheme.size(); ++i) {
        column_index[scheme[i].GetName()] = i;
    }

    if (!column_names_) {
        column_names_.emplace();
        for (auto column : scheme) {
            column_names_->push_back(column.GetName());
        }
    }

    std::set<std::string> columns_names_set;
    for (const auto& column_name : *column_names_) {
        if (!column_index.contains(column_name)) {
            throw std::runtime_error("Column does not exist");
        }
        columns_names_set.insert(column_name);
    }

    std::vector<Column> new_scheme;
    for (const auto& column_name : *column_names_) {
        new_scheme.push_back(scheme[column_index[column_name]]);
    }

    auto new_table = std::make_unique<Table>();
    new_table->SetScheme(std::move(new_scheme));
    for (const auto& row : table->GetRows()) {
        const auto& expression_result = where_expression_.Calc(row);
        if (!std::holds_alternative<bool>(expression_result)) {
            throw std::runtime_error("Where expression does not match expected format");
        }
        if (!std::get<bool>(expression_result)) {
            continue;
        }
        std::vector<std::unique_ptr<Cell>> new_cells;
        for (const auto& column_name : *column_names_) {
            new_cells.emplace_back(row.GetCells()[column_index[column_name]]->Clone());
        }
        Row new_row(scheme);
        new_row.SetCells(std::move(new_cells));
        new_table->AddRow(std::move(new_row));
    }
    return new_table;
}
