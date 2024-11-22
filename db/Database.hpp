#pragma once

#include "Ident.hpp"
#include "Row.hpp"
#include "queries.hpp"

#include <memory>
#include <string_view>
#include <unordered_map>

class Table;
class TempTable;

class Database {
    public:
        Database();

        bool has_table(const Ident& name) const;
        Table* get_table(const Ident& name) const;

        void push_table(Table table);
        void remove_table(const Ident& name);

        void create_table_query(CreateTableQuery);
        void drop_table_query(DropTableQuery);
        void insert_query(InsertQuery);
        TempTable select_query(SelectQuery) const;
        size_t update_query(UpdateQuery);
        size_t delete_query(DeleteQuery);

        QueryResult query(AnyQuery query);
        QueryResult query(std::string_view query);

        std::vector<QueryResult> queries(std::vector<AnyQuery> queries);
        std::vector<QueryResult> queries(std::string_view queries);

    private:
        std::unordered_map<Ident, std::unique_ptr<Table>> tables;
};
