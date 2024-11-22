#include "Database.hpp"

#include "Table.hpp"
#include "parsing.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
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
void Database::delete_query(DeleteQuery q) {
    get_table(q.table_name)->delete_rows(std::move(q.cond));
}

std::optional<std::vector<Row*>> Database::query(AnyQuery query) {
    if (auto* q = std::get_if<CreateTableQuery>(&query)) {
        create_table_query(std::move(*q));
        return std::nullopt;
    } else if (auto* q = std::get_if<DropTableQuery>(&query)) {
        drop_table_query(std::move(*q));
        return std::nullopt;
    } else if (auto* q = std::get_if<SelectQuery>(&query)) {
        return {select_query(std::move(*q))};
    } else if (auto* q = std::get_if<InsertQuery>(&query)) {
        return {insert_query(std::move(*q))};
    } else if (auto* q = std::get_if<UpdateQuery>(&query)) {
        return {update_query(std::move(*q))};
    } else if (auto* q = std::get_if<DeleteQuery>(&query)) {
        delete_query(std::move(*q));
        return std::nullopt;
    } else {
        assert(false);
    }
}

std::optional<std::vector<Row*>> Database::query(std::string_view q) {
    return query(parser::parse(query_parser, q));
}

std::vector<std::vector<Row*>> Database::queries(std::vector<AnyQuery> queries) {
    std::vector<std::vector<Row*>> ans;

    for (auto& q : queries) {
        auto query_ans = query(std::move(q));
        if (query_ans.has_value()) {
            ans.push_back(std::move(*query_ans));
        }
    }

    return ans;
}

std::vector<std::vector<Row*>> Database::queries(std::string_view q) {
    return queries(parser::parse(queries_parser, q));
}
