#pragma once

#include "Ident.hpp"
#include "Row.hpp"
#include "queries.hpp"

#include <memory>
#include <string_view>
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
        void delete_query(DeleteQuery);

        std::optional<std::vector<Row*>> query(AnyQuery query);
        std::optional<std::vector<Row*>> query(std::string_view query);

        std::vector<std::vector<Row*>> queries(std::vector<AnyQuery> queries);
        std::vector<std::vector<Row*>> queries(std::string_view queries);

    private:
        std::unordered_map<Ident, std::unique_ptr<Table>> tables;
};
