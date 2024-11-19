#include "CellType.h"
#include <fstream>

CellType::~CellType() noexcept {}

void CellTypeInt32::Encode(std::ofstream &out) {
    out << value_ << "\n";
}

void CellTypeBool::Encode(std::ofstream &out) {
    out << value_ << "\n";
}

void CellTypeString::Encode(std::ofstream &out) {
    out << size_ << " " << *value_ << "\n";
}

void CellTypeBytes::Encode(std::ofstream &out) {
    out << size_ << " ";
    for (auto ch : *value_) {
        out << ch << " ";
    }
    out << "\n";
}

void CellTypeInt32::Decode(std::ifstream &in) {
    in >> value_;
}

void CellTypeBool::Decode(std::ifstream &in) {
    in >> value_;
}

void CellTypeString::Decode(std::ifstream &in) {
    std::string value;
    in >> size_ >> value;
    value_ = std::make_unique<std::string>(value);
}

void CellTypeBytes::Decode(std::ifstream &in) {
    in >> size_;
    for (auto& ch : *value_) {
        in >> ch;
    }
}