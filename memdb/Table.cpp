#include "Table.h"

#include <iostream>

const std::vector<Column>& Table::GetScheme() const {
    return scheme_;
}

const std::vector<Row>& Table::GetRows() const {
    return rows_;
}

std::vector<Row>& Table::GetMutableRows() {
    return rows_;
}

void Table::SetScheme(std::vector<Column>&& scheme) {
    scheme_ = std::move(scheme);
}

void Table::SetRows(std::vector<Row>&& rows) {
    rows_ = std::move(rows);
}

void Table::AddRow(Row&& row) {
    rows_.push_back(std::move(row));
}
