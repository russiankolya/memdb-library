#pragma once
#include <string>
#include <set>
#include "DataType.h"

class Column {
public:
    enum class Type { Int32Type, BoolType, StringType, BytesType };

    enum class Attribute { Unique, Autoincrement, Key };

    const std::string& GetName() const;
    Type GetType() const;
    const std::unique_ptr<DataType>& GetDefaultValue() const;
    const std::set<Attribute>& GetAttributes() const;

    void SetName(const std::string&& name);
    void SetType(const Type type);
    void SetDefaultValue(std::unique_ptr<DataType>&& default_value);
    void SetAttributes(std::set<Attribute>&& attributes);

private:
    std::string name_;
    Type type_{};
    std::unique_ptr<DataType> default_value_;
    std::set<Attribute> attributes_;
};