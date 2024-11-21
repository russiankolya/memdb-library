#pragma once
#include <map>
#include <memory>

#include "Table.h"

class Database {
public:
    void LoadFromFile(std::ifstream &in);
    void SaveToFile(std::ofstream &out);

private:
    std::map<std::string, std::unique_ptr<Table>> current_tables_;
};
