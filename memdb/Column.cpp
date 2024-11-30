#include "Column.h"

#include <set>

Column::Column(const Column& other) {
    name_ = other.name_;
    type_ = other.type_;
    if (other.default_value_) {
        default_value_ = other.default_value_->Clone();
    }
    attributes_ = other.attributes_;
}

Column& Column::operator=(const Column& other) {
    if (this != &other) {
        name_ = other.name_;
        type_ = other.type_;
        if (default_value_) {
            default_value_ = other.default_value_->Clone();
        }
        attributes_ = other.attributes_;
    }
    return *this;
}

const std::string& Column::GetName() const {
    return name_;
}

std::pair<Cell::Type, std::optional<size_t>> Column::GetType() const {
    return type_;
}

const std::unique_ptr<Cell>& Column::GetDefaultValue() const {
    return default_value_;
}

const std::set<Column::Attribute>& Column::GetAttributes() const {
    return attributes_;
}

void Column::SetName(const std::string&& name) {
    name_ = std::move(name);
}

void Column::SetType(const Cell::Type type) {
    type_.first = type;
}

void Column::SetType(Cell::Type type, size_t size) {
    type_ = {type, size};
}

void Column::SetDefaultValue(std::unique_ptr<Cell>&& default_value) {
    default_value_ = std::move(default_value);
}

void Column::SetAttributes(std::set<Attribute>&& attributes) {
    attributes_ = std::move(attributes);
}

void Column::AddAttribute(const Attribute attribute) {
    attributes_.insert(attribute);
}
