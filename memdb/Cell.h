#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

class Cell {
public:
    enum class Type { Int32Type, BoolType, StringType, BytesType };

    virtual ~Cell() noexcept;

    virtual Type GetType() = 0;
    virtual std::optional<size_t> GetSize() = 0;

    virtual void Encode(std::ofstream& out) = 0;
    virtual void Decode(std::ifstream& in) = 0;
};

class CellInt32 final : public Cell {
public:
    CellInt32() noexcept = default;
    explicit CellInt32(int32_t value) noexcept;

    int32_t GetValue() const noexcept;
    Type GetType() override;
    std::optional<size_t> GetSize() override;

    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;

private:
    int32_t value_{};
};

class CellBool final : public Cell {
public:
    CellBool() noexcept = default;
    explicit CellBool(bool value) noexcept;

    bool GetValue() const noexcept;
    Type GetType() override;
    std::optional<size_t> GetSize() override;

    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;

private:
    bool value_{};
};

class CellString final : public Cell {
public:
    CellString() noexcept = default;
    explicit CellString(const std::string& value) noexcept;

    const std::string& GetValue() const noexcept;
    Type GetType() override;
    std::optional<size_t> GetSize() override;

    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;

private:
    std::unique_ptr<std::string> value_;
};

class CellBytes final : public Cell {
public:
    CellBytes() noexcept = default;
    explicit CellBytes(const std::vector<uint8_t>& value) noexcept;

    const std::vector<uint8_t>& GetValue() const noexcept;
    Type GetType() override;
    std::optional<size_t> GetSize() override;

    void Encode(std::ofstream& out) override;
    void Decode(std::ifstream& in) override;

private:
    std::unique_ptr<std::vector<uint8_t>> value_;
};