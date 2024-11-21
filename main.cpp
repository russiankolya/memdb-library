#define private public
#include "memdb/Database.h"
#include <fstream>
#include <iostream>
#include "Tokenizer.h"

std::string StringByTokenType(Token::Type type) {
    std::string result;
    switch (type) {
        case Token::Type::Keyword:
            result = "Keyword";
            break;
        case Token::Type::LogicKeyword:
            result = "LogicKeyword";
            break;
        case Token::Type::Operator:
            result = "Operator";
            break;
        case Token::Type::Delimiter:
            result = "Delimiter";
            break;
        case Token::Type::Identifier:
            result = "Identifier";
            break;
        case Token::Type::Number:
            result = "Number";
            break;
        case Token::Type::Bool:
            result = "Bool";
            break;
        case Token::Type::String:
            result = "String";
            break;
        case Token::Type::Bytes:
            result = "Bytes";
            break;
    }
    return result;
}

int main() {
    std::cout << 0x1 << "\n";
    Tokenizer tokenizer("select id, is_admin from users where name = \"orion\" or where not is_admin and age > 30");
    for (auto x = tokenizer.Tokenize(); auto it : x) {
        std::cout << "%" << it.value_ << "%     " << StringByTokenType(it.type_) << "\n";
    }

    Database db;
    db.current_tables_["ab123"] = std::make_unique<Table>();

    Column column1;
    column1.name_ = "col_int1";
    column1.type_ = Column::Type::Int32Type;
    DataTypeInt32 default_col_int1_value;
    default_col_int1_value.value_ = 234;
    column1.default_value_ = std::make_unique<DataTypeInt32>(std::move(default_col_int1_value));
    column1.attributes_.insert(Column::Attribute::Autoincrement);
    column1.attributes_.insert(Column::Attribute::Unique);
    db.current_tables_["ab123"]->scheme_.emplace_back(std::move(column1));

    Column column2;
    column2.name_ = "col_string1";
    column2.type_ = Column::Type::StringType;
    DataTypeString default_col_string1_value;
    default_col_string1_value.size_ = 5;
    default_col_string1_value.value_ = std::make_unique<std::string>("abcd");
    column2.default_value_ = std::make_unique<DataTypeString>(std::move(default_col_string1_value));
    column2.attributes_.insert(Column::Attribute::Key);
    column2.attributes_.insert(Column::Attribute::Unique);
    db.current_tables_["ab123"]->scheme_.emplace_back(std::move(column2));


    DataTypeInt32 a11;
    a11.value_ = 543;
    DataTypeString a12;
    a12.size_ = 5;
    a12.value_ = std::make_unique<std::string>("fwefw");
    DataTypeInt32 a21;
    a21.value_ = 895;
    DataTypeString a22;
    a22.size_ = 5;
    a22.value_ = std::make_unique<std::string>("cbsdf");

    Row r1(std::move(db.current_tables_["ab123"]->scheme_));
    r1.cells_.emplace_back(std::make_unique<DataTypeInt32>(std::move(a11)));
    r1.cells_.emplace_back(std::make_unique<DataTypeString>(std::move(a12)));
    db.current_tables_["ab123"]->rows_.emplace_back(std::move(r1));

    Row r2(std::move(db.current_tables_["ab123"]->scheme_));
    r2.cells_.emplace_back(std::make_unique<DataTypeInt32>(std::move(a21)));
    r2.cells_.emplace_back(std::make_unique<DataTypeString>(std::move(a22)));
    db.current_tables_["ab123"]->rows_.emplace_back(std::move(r2));

    std::ofstream my_file("db.txt");
    db.SaveToFile(my_file);
    my_file.close();

    Database copy_db;
    std::ifstream my_file_copy("db.txt");
    copy_db.LoadFromFile(my_file_copy);

    std::ofstream my_file1("db1.txt");
    copy_db.SaveToFile(my_file1);

    std::cout << "!\n";
}