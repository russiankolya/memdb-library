#pragma once
#include <cstdint>
#include <memory>
#include <string>

class CellType {
public:
    virtual ~CellType() noexcept;

    virtual void Encode(std::ofstream &out) = 0;
    virtual void Decode(std::ifstream &in) = 0;
};

class CellTypeInt32 : public CellType {
public:
    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;
private:
    int32_t value_;
};

class CellTypeBool : public CellType {
public:
    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;
private:
    bool value_;
};

class CellTypeString : public CellType {
public:
    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;
private:
    std::unique_ptr<std::string> value_;
    size_t size_;
};

class CellTypeBytes : public CellType {
public:
    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;
private:
    std::unique_ptr<std::vector<char>> value_;
    size_t size_;
};