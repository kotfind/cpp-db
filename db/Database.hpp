#pragma once

#include "Ident.hpp"
#include "Row.hpp"
#include "queries.hpp"

#include <memory>
#include <unordered_map>

class Table;

class Database {
    public:
        Database();

        bool has_table(const Ident& name) const;
        Table* get_table(const Ident& name) const;

        void push_table(Table table);
        void remove_table(const Ident& name);

        void create_table_query(CreateTableQuery);
        void drop_table_query(DropTableQuery);
        std::vector<Row*> insert_query(InsertQuery);
        std::vector<Row*> select_query(SelectQuery) const;
        std::vector<Row*> update_query(UpdateQuery);
        size_t delete_query(DeleteQuery);

    private:
        std::unordered_map<Ident, std::unique_ptr<Table>> tables;
};
