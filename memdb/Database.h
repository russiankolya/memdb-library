#pragma once
#include <Response.h>
#include <Table.h>

#include <map>

class Database {
public:
    void LoadFromFile(std::ifstream &in);
    void SaveToFile(std::ofstream &out);

    Response Execute(const std::string &query);

private:
    std::map<std::string, std::unique_ptr<Table>> current_tables_;
};
