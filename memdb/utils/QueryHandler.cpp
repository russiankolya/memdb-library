#include "QueryHandler.h"
#include <sstream>

QueryHandler::QueryHandler(const std::vector<Token>& tokens) : tokens_(tokens) {}

int8_t ConvertCharToNumber(char ch) {
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }
    if (ch >= 'a' && ch <= 'z') {
        return ch - 'a' + 10;
    }
    if (ch >= 'A' && ch <= 'Z') {
        return ch - 'A' + 10;
    }
    throw std::invalid_argument("Invalid hex character '" + std::string(1, ch) + "'");
}

std::vector<uint8_t> QueryHandler::HexStringToVector(const std::string& hex_string) {
    std::vector<uint8_t> result((hex_string.size() - 1) / 2);
    if (hex_string.size() % 2 != 0) {
        result[0] = ConvertCharToNumber(hex_string[2]);
    }
    for (size_t i = 2 + hex_string.size() % 2; i < hex_string.size(); i += 2) {
        result[(i - 1) / 2] = ConvertCharToNumber(hex_string[i]) * 16 + ConvertCharToNumber(hex_string[i + 1]);
    }
    return result;
}
