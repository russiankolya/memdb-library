#pragma once
#include <vector>

#include "Column.h"
#include "Row.h"

class Table {
public:
    const std::vector<Column>& GetScheme() const;
    const std::vector<Row>& GetRows() const;

    void SetScheme(std::vector<Column>&& scheme);
    void AddRow(Row&& row);

private:
    std::vector<Column> scheme_;
    std::vector<Row> rows_;
};
