#pragma once

#include "Expression.h"
#include "QueryHandler.h"

class SelectQueryHandler final : public QueryHandler {
public:
    explicit SelectQueryHandler(const std::vector<Token>& tokens);

    void Parse() override;
    std::unique_ptr<Table> Execute(std::map<std::string, std::unique_ptr<Table>>& current_tables) override;

private:
    std::string table_name_;
    std::optional<std::vector<std::string>> column_names_;
    Expression where_expression_;
};
