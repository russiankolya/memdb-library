#pragma once
#include "Expression.h"
#include "QueryHandler.h"

class DeleteQueryHandler final : public QueryHandler {
public:
    explicit DeleteQueryHandler(const std::vector<Token>& tokens);

    void Parse() override;
    std::unique_ptr<Table> Execute(
        std::map<std::string, std::unique_ptr<Table>>& current_tables) override;

private:
    std::string table_name_;
    Expression where_expression_;
};
