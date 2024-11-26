#include "Cell.h"

#include <Token.h>

#include <fstream>

Cell::~Cell() noexcept {
}

CellInt32::CellInt32(const int32_t value) noexcept : value_(value) {
}

CellBool::CellBool(const bool value) noexcept : value_(value) {
}

CellString::CellString(const std::string &value) noexcept
    : value_(std::make_unique<std::string>(value)) {
}

CellBytes::CellBytes(const std::vector<uint8_t> &value) noexcept
    : value_(std::make_unique<std::vector<uint8_t>>(value)) {
}

int32_t CellInt32::GetValue() const noexcept {
    return value_;
}

bool CellBool::GetValue() const noexcept {
    return value_;
}

const std::string &CellString::GetValue() const noexcept {
    return *value_;
}

const std::vector<uint8_t> &CellBytes::GetValue() const noexcept {
    return *value_;
}

void CellInt32::Encode(std::ofstream &out) {
    out << value_ << "\n";
}

void CellBool::Encode(std::ofstream &out) {
    out << value_ << "\n";
}

void CellString::Encode(std::ofstream &out) {
    out << *value_ << "\n";
}

void CellBytes::Encode(std::ofstream &out) {
    out << value_->size() << " ";
    for (const auto ch : *value_) {
        out << ch << " ";
    }
    out << "\n";
}

void CellInt32::Decode(std::ifstream &in) {
    in >> value_;
}

void CellBool::Decode(std::ifstream &in) {
    in >> value_;
}

void CellString::Decode(std::ifstream &in) {
    std::string value;
    in >> value;
    value_ = std::make_unique<std::string>(value);
}

void CellBytes::Decode(std::ifstream &in) {
    // TODO: Add decode for Bytes
}