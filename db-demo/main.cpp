#include <cassert>
#include <db.hpp>
#include <iostream>
#include <ostream>
#include <variant>

void exec_query(Database& db, const std::string& query) {
    auto query_res = db.query(query);

    if (auto* tab = std::get_if<TempTable>(&query_res)) {
        std::cout << *tab << std::endl;
    } else if (auto* rows_affected = std::get_if<size_t>(&query_res)) {
        std::cout << rows_affected << " ROWS AFFECTED." << std::endl;
    } else if (auto* none = std::get_if<std::tuple<>>(&query_res)) {
        std::cout << "OK" << std::endl;
    } else {
        assert(false);
    }
}

int main() {
    Database db;

    std::string query_buf;
    while (!std::cin.eof()) {
        char c = std::cin.get();
        if (c == ';') {
            exec_query(db, query_buf);
            query_buf.clear();
        } else {
            query_buf += c;
        }
    }
}
