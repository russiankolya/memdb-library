#include "Database.h"

int main() {
    Database db;
    db.Execute("CREATE TABLE t ({unique} a : int32, b : bool, c : bytes[3], d : string[5])");
    db.Execute("INSERT (3, true, 0x121212, \"abcde\") TO t");
    const auto& result = db.Execute("SELECT * FROM t WHERE true");
    return 0;
}