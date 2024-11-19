#pragma once
#include "CellType.h"
#include "Column.h"
#include <vector>

class Row {
public:
    Row(const std::vector<Column>& scheme);

    const std::vector<std::unique_ptr<CellType>>& GetCells() const;
    const std::vector<Column>& GetScheme() const;

    void AddCell(std::unique_ptr<CellType>&& cell);
private:
    std::vector<std::unique_ptr<CellType>> cells_;
    const std::vector<Column> &scheme_;
};
