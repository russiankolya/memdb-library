#pragma once
#include <string>
#include <set>
#include "CellType.h"

class Column {
public:
    enum class Type { Int32Type, BoolType, StringType, BytesType };

    enum class Attribute { Unique, Autoincrement, Key };

    const std::string& GetName() const;
    const Type GetType() const;
    const std::unique_ptr<CellType>& GetDefaultValue() const;
    const std::set<Attribute>& GetAttributes() const;

    void SetName(const std::string&& name);
    void SetType(const Type type);
    void SetDefaultValue(std::unique_ptr<CellType>&& default_value);
    void SetAttributes(std::set<Attribute>&& attributes);

private:
    std::string name_;
    Type type_{};
    std::unique_ptr<CellType> default_value_;
    std::set<Attribute> attributes_;
};