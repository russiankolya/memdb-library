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
    : value_(std::make_shared<std::string>(value)) {
}

CellBytes::CellBytes(const std::vector<uint8_t> &value) noexcept
    : value_(std::make_shared<std::vector<uint8_t>>(value)) {
}

CellString::CellString(const std::shared_ptr<std::string> &value) : value_(value) {
}

CellBytes::CellBytes(const std::shared_ptr<std::vector<uint8_t>> &value) : value_(value) {
}

std::unique_ptr<Cell> CellInt32::Clone() const {
    return std::make_unique<CellInt32>(value_);
}

std::unique_ptr<Cell> CellBool::Clone() const {
    return std::make_unique<CellBool>(value_);
}

std::unique_ptr<Cell> CellString::Clone() const {
    return std::make_unique<CellString>(value_);
}

std::unique_ptr<Cell> CellBytes::Clone() const {
    return std::make_unique<CellBytes>(value_);
}

std::variant<int32_t, bool, std::string, std::vector<uint8_t>> CellInt32::GetValue() const {
    return value_;
}

std::variant<int32_t, bool, std::string, std::vector<uint8_t>> CellBool::GetValue() const {
    return value_;
}

std::variant<int32_t, bool, std::string, std::vector<uint8_t>> CellString::GetValue() const {
    return *value_;
}

std::variant<int32_t, bool, std::string, std::vector<uint8_t>> CellBytes::GetValue() const {
    return *value_;
}

Cell::Type CellInt32::GetType() {
    return Type::Int32Type;
}

Cell::Type CellBool::GetType() {
    return Type::BoolType;
}

Cell::Type CellString::GetType() {
    return Type::StringType;
}

Cell::Type CellBytes::GetType() {
    return Type::BytesType;
}

std::optional<size_t> CellInt32::GetSize() {
    return std::nullopt;
}

std::optional<size_t> CellBool::GetSize() {
    return std::nullopt;
}

std::optional<size_t> CellString::GetSize() {
    return value_->size();
}

std::optional<size_t> CellBytes::GetSize() {
    return value_->size();
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