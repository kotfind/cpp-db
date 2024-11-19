#include "Database.hpp"

#include "Table.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <variant>

Database::Database() {}

bool Database::has_table(const Ident& name) const {
    return tables.contains(name);
}

Table* Database::get_table(const Ident& name) const {
    if (!has_table(name)) {
        throw std::runtime_error("table with name " + name.get_inner() + " does not exist");
    }

    return tables.at(name).get();
}

void Database::push_table(Table table) {
    const auto name = table.get_name();
    if (has_table(name)) {
        throw std::runtime_error("table with name " + name.get_inner() + " already exist");
    }

    tables[name] = std::make_unique<Table>(std::move(table));
}
void Database::remove_table(const Ident& name) {
    if (!has_table(name)) {
        throw std::runtime_error("table with name " + name.get_inner() + " does not exist");
    }

    tables.erase(name);
}

void Database::create_table_query(CreateTableQuery q) {
    push_table(Table(std::move(q.table_name), std::move(q.columns)));
}
void Database::drop_table_query(DropTableQuery q) {
    remove_table(q.table_name);
}
std::vector<Row*> Database::insert_query(InsertQuery q) {
    auto* table = get_table(q.table_name);
    if (auto* named = std::get_if<RowInitializerNamed>(&q.row)) {
        return {table->insert_row_named(std::move(*named))};
    } else if (auto* positioned = std::get_if<RowInitializerPositioned>(&q.row)) {
        return {table->insert_row_positioned(std::move(*positioned))};
    } else {
        // Unreachable
        assert(false);
    }
}
std::vector<Row*> Database::select_query(SelectQuery q) const {
    return get_table(q.table_name)->select_rows(std::move(q.cond));
}
std::vector<Row*> Database::update_query(UpdateQuery q) {
    return get_table(q.table_name)->update_rows(std::move(q.assignments), std::move(q.cond));
}
size_t Database::delete_query(DeleteQuery q) {
    return get_table(q.table_name)->delete_rows(std::move(q.cond));
}
