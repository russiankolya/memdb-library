#include "Row.h"

Row::Row(const std::vector<Column> &scheme) : scheme_(scheme) {}

const std::vector<std::unique_ptr<CellType>>& Row::GetCells() const {
    return cells_;
}

const std::vector<Column> &Row::GetScheme() const {
    return scheme_;
}

void Row::AddCell(std::unique_ptr<CellType> &&cell) {
    cells_.push_back(std::move(cell));
}