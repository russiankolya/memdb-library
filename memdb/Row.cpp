#include "Row.h"

Row::Row(const std::vector<Column> &scheme) : scheme_(scheme) {
}

const std::vector<std::unique_ptr<Cell>> &Row::GetCells() const {
    return cells_;
}

const std::vector<Column> &Row::GetScheme() const {
    return scheme_;
}

void Row::SetCells(std::vector<std::unique_ptr<Cell>> &&cells) {
    cells_ = std::move(cells);
}

void Row::SetCell(const size_t index, std::unique_ptr<Cell> &&cell) {
    cells_[index] = std::move(cell);
}


void Row::AddCell(std::unique_ptr<Cell> &&cell) {
    cells_.push_back(std::move(cell));
}