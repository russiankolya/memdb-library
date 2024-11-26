#pragma once
#include <set>
#include <string>

#include "Cell.h"

class Column {
public:
    enum class Attribute { Unique, Autoincrement, Key };

    const std::string& GetName() const;
    std::pair<Cell::Type, std::optional<size_t>> GetType() const;
    const std::unique_ptr<Cell>& GetDefaultValue() const;
    const std::set<Attribute>& GetAttributes() const;

    void SetName(const std::string&& name);
    void SetType(Cell::Type type);
    void SetType(Cell::Type type, size_t size);
    void SetDefaultValue(std::unique_ptr<Cell>&& default_value);
    void SetAttributes(std::set<Attribute>&& attributes);

    void AddAttribute(Attribute attribute);

private:
    std::string name_;
    std::pair<Cell::Type, std::optional<size_t>> type_{};
    std::unique_ptr<Cell> default_value_;
    std::set<Attribute> attributes_;
};