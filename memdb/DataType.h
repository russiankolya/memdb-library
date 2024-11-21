#pragma once
#include <cstdint>
#include <memory>
#include <string>

class DataType {
public:
    virtual ~DataType() noexcept;

    virtual void Encode(std::ofstream &out) = 0;
    virtual void Decode(std::ifstream &in) = 0;
};

class DataTypeInt32 final : public DataType {
public:
    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;
private:
    int32_t value_{};
};

class DataTypeBool final : public DataType {
public:
    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;
private:
    bool value_{};
};

class DataTypeString final : public DataType {
public:
    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;
private:
    std::unique_ptr<std::string> value_;
    size_t size_{};
};

class DataTypeBytes final : public DataType {
public:
    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;
private:
    std::unique_ptr<std::vector<char>> value_;
    size_t size_{};
};