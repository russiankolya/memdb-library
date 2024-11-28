#pragma once
#include <vector>

#include "Column.h"

class Row {
public:
    explicit Row(const std::vector<Column>& scheme);

    const std::vector<std::unique_ptr<Cell>>& GetCells() const;
    const std::vector<Column>& GetScheme() const;

    void SetCells(std::vector<std::unique_ptr<Cell>>&& cells);
    void AddCell(std::unique_ptr<Cell>&& cell);

private:
    std::vector<std::unique_ptr<Cell>> cells_;
    const std::vector<Column>& scheme_;
};
