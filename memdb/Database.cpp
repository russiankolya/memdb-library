#include "Database.h"
#include <fstream>
#include <set>

std::unique_ptr<DataType> GetPointerByType(Column::Type type) {
    switch (type) {
        case Column::Type::Int32Type:
            return std::make_unique<DataTypeInt32>();
        case Column::Type::BoolType:
            return std::make_unique<DataTypeBool>();
        case Column::Type::StringType:
            return std::make_unique<DataTypeString>();
        case Column::Type::BytesType:
            return std::make_unique<DataTypeBytes>();
        default:
            throw std::runtime_error("Unknown column type");
    }
}

void Database::LoadFromFile(std::ifstream &in) {
    size_t tables_count;
    in >> tables_count;
    for (size_t table_index = 0; table_index < tables_count; table_index++) {
        std::string table_name;
        in >> table_name;
        current_tables_[table_name] = std::make_unique<Table>();
        size_t scheme_size;
        in >> scheme_size;
        std::vector<Column> scheme;
        for (size_t column_index = 0; column_index < scheme_size; column_index++) {
            Column column;
            std::string column_name;
            in >> column_name;
            column.SetName(std::move(column_name));
            size_t type;
            in >> type;
            column.SetType(static_cast<Column::Type>(type));
            auto default_value = GetPointerByType(column.GetType());
            default_value->Decode(in);
            column.SetDefaultValue(std::move(default_value));
            size_t attributes_count;
            in >> attributes_count;

            std::set<Column::Attribute> attributes;

            for (size_t attribute_index = 0; attribute_index < attributes_count; attribute_index++) {
                size_t attribute;
                in >> attribute;
                attributes.insert(static_cast<Column::Attribute>(attribute));
            }
            column.SetAttributes(std::move(attributes));
            scheme.emplace_back(std::move(column));
        }
        current_tables_[table_name]->SetScheme(std::move(scheme));
        size_t rows_count;
        in >> rows_count;
        for (size_t row_index = 0; row_index < rows_count; row_index++) {
            Row row(current_tables_[table_name]->GetScheme());
            for (size_t cell_index = 0; cell_index < scheme_size; cell_index++) {
                auto cell = GetPointerByType(row.GetScheme()[cell_index].GetType());
                cell->Decode(in);
                row.AddCell(std::move(cell));
            }
            current_tables_[table_name]->AddRow(std::move(row));
        }
    }
}

void Database::SaveToFile(std::ofstream &out) {
    out << current_tables_.size() << "\n";
    for (const auto &[name, table] : current_tables_) {
        out << name << "\n";
        const auto &scheme = table->GetScheme();
        out << scheme.size() << "\n";
        for (const auto &column : scheme) {
            out << column.GetName() << "\n";
            out << static_cast<size_t>(column.GetType()) << "\n";
            column.GetDefaultValue()->Encode(out);
            const auto &attributes = column.GetAttributes();
            out << attributes.size() << "\n";
            for (auto attribute : attributes) {
                out << static_cast<size_t>(attribute) << " ";
            }
            out << "\n";
        }
        const auto &rows = table->GetRows();
        out << rows.size() << "\n";
        for (const auto &row : rows) {
            for (const auto &cell : row.GetCells()) {
                cell->Encode(out);
            }
        }
    }
}
