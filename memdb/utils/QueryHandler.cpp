#include "QueryHandler.h"
#include <sstream>

QueryHandler::QueryHandler(const std::vector<Token>& tokens) : tokens_(tokens) {}

std::vector<uint8_t> QueryHandler::HexStringToVector(const std::string& hex_string) {
    // TODO: Write it
    return {82};
}
