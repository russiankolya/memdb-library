#include "DataType.h"
#include <fstream>

DataType::~DataType() noexcept {}

void DataTypeInt32::Encode(std::ofstream &out) {
    out << value_ << "\n";
}

void DataTypeBool::Encode(std::ofstream &out) {
    out << value_ << "\n";
}

void DataTypeString::Encode(std::ofstream &out) {
    out << size_ << " " << *value_ << "\n";
}

void DataTypeBytes::Encode(std::ofstream &out) {
    out << size_ << " ";
    for (auto ch : *value_) {
        out << ch << " ";
    }
    out << "\n";
}

void DataTypeInt32::Decode(std::ifstream &in) {
    in >> value_;
}

void DataTypeBool::Decode(std::ifstream &in) {
    in >> value_;
}

void DataTypeString::Decode(std::ifstream &in) {
    std::string value;
    in >> size_ >> value;
    value_ = std::make_unique<std::string>(value);
}

void DataTypeBytes::Decode(std::ifstream &in) {
    in >> size_;
    for (auto& ch : *value_) {
        in >> ch;
    }
}