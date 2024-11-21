#include "Column.h"

#include <set>

const std::string& Column::GetName() const {
    return name_;
}

Column::Type Column::GetType() const {
    return type_;
}

const std::unique_ptr<DataType>& Column::GetDefaultValue() const {
    return default_value_;
}

const std::set<Column::Attribute>& Column::GetAttributes() const {
    return attributes_;
}

void Column::SetName(const std::string&& name) {
    name_ = name;
}

void Column::SetType(const Column::Type type) {
    type_ = type;
}

void Column::SetDefaultValue(std::unique_ptr<DataType>&& default_value) {
    default_value_ = std::move(default_value);
}

void Column::SetAttributes(std::set<Column::Attribute>&& attributes) {
    attributes_ = std::move(attributes);
}