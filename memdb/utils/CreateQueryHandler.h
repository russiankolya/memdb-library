#pragma once
#include "Column.h"
#include "utils/QueryHandler.h"

class CreateQueryHandler final : public QueryHandler {
public:
    explicit CreateQueryHandler(const std::vector<Token>& tokens);

    void Parse() override;
    Response Execute(std::map<std::string, std::unique_ptr<Table>> &current_tables) override;

private:
    enum class State {
        Base,
        WaitForAttribute,
        WaitForColumnName,
        WaitForDataType,
        WaitForDefaultValue,
        End
    };

    std::string table_name_;
    std::vector<Column> scheme_;
};
