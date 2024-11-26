#pragma once
#include <map>
#include <vector>

#include "Table.h"
#include "Response.h"
#include "Token.h"

class QueryHandler {
public:
    explicit QueryHandler(const std::vector<Token>& tokens);
    virtual ~QueryHandler() noexcept = default;

    virtual void Parse() = 0;
    virtual Response Execute(std::map<std::string, std::unique_ptr<Table>> &current_tables) = 0;

    static std::vector<uint8_t> HexStringToVector(const std::string& hex_string);
protected:
    const std::vector<Token>& tokens_;
};
