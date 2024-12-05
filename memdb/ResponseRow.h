#pragma once
#include <map>
#include <string>
#include <vector>
#include <variant>

class ResponseRow {
public:
    template<typename Type>
    Type Get(const std::string& key) const;

    friend class Database;
private:
    void Set(const std::string& key, const std::variant<int, bool, std::string, std::vector<uint8_t>>& value);

    std::map<std::string, std::variant<int, bool, std::string, std::vector<uint8_t>>> value_by_column_;
};
