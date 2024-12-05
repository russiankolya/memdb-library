#include "ResponseRow.h"

void ResponseRow::Set(const std::string& key, const std::variant<int, bool, std::string, std::vector<uint8_t>> &value) {
    value_by_column_[key] = value;
}

template <typename Type>
Type ResponseRow::Get(const std::string& key) const {
    return std::get<Type>(value_by_column_.at(key));
}

