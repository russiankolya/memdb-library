#pragma once
#include <vector>

#include "Column.h"
#include "DataType.h"

class Row {
public:
    explicit Row(const std::vector<Column>& scheme);

    const std::vector<std::unique_ptr<DataType>>& GetCells() const;
    const std::vector<Column>& GetScheme() const;

    void AddCell(std::unique_ptr<DataType>&& cell);

private:
    std::vector<std::unique_ptr<DataType>> cells_;
    const std::vector<Column>& scheme_;
};
