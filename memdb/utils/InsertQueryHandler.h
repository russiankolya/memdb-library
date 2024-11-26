#pragma once
#include <unordered_map>
#include <variant>

#include "QueryHandler.h"

class InsertQueryHandler : public QueryHandler {
public:
    InsertQueryHandler(const std::vector<Token>& tokens);

    void Parse() override;
    Response Execute(std::map<std::string, std::unique_ptr<Table>> &current_tables) override;

private:
    std::string table_name_;
    std::variant<std::vector<std::unique_ptr<Cell>>,
                 std::map<std::string, std::unique_ptr<Cell>>>
        parsed_row_;
};
