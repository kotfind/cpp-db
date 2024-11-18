#include "Expr.hpp"
#include "macro.hpp"
#include "test_utils.hpp"
#include "Table.hpp"
#include "Value.hpp"
#include <memory>
#include <unordered_set>

TEST_GROUP(table)

static Table get_empty_table() {
    Table table(Ident("people"), {
        Column(Ident("name"), ValueType::STRING),
        Column(Ident("is_male"), ValueType::BOOL),
    });
    return table;
}

TEST(name, table)
    ASSERT_EQ(get_empty_table().get_name(), Ident("people"));
END_TEST

TEST(get_columns, table)
    auto columns = get_empty_table().get_columns();

    ASSERT_EQ(columns[0], Column(Ident("name"), ValueType::STRING));
    ASSERT_EQ(columns[1], Column(Ident("is_male"), ValueType::BOOL));
END_TEST

TEST(get_types, table)
    auto types = get_empty_table().get_types();

    ASSERT_EQ(types[0], ValueType::STRING);
    ASSERT_EQ(types[1], ValueType::BOOL);
END_TEST

static Table get_filled_table() {
    Table table(Ident("people"), {
        Column(Ident("name"), ValueType::STRING),
        Column(Ident("is_male"), ValueType::BOOL),
    });

    table.push_row_positioned({{Value::from_string("Ivan"), Value::from_bool(true)}});
    table.push_row_positioned({{Value::from_string("Ann"), Value::from_bool(false)}});
    table.push_row_positioned({{Value::from_string("Jim"), Value::from_bool(true)}});

    return table;
}

TEST(rows, table)
    auto table = get_filled_table();
    auto& rows = table.get_rows();

    ASSERT_EQ(rows[0]->get_id(), 1);
    ASSERT_EQ((*rows[0])[0], Value::from_string("Ivan"));
    ASSERT_EQ((*rows[0])[1], Value::from_bool(true));

    ASSERT_EQ(rows[1]->get_id(), 2);
    ASSERT_EQ((*rows[1])[0], Value::from_string("Ann"));
    ASSERT_EQ((*rows[1])[1], Value::from_bool(false));

    ASSERT_EQ(rows[2]->get_id(), 3);
    ASSERT_EQ((*rows[2])[0], Value::from_string("Jim"));
    ASSERT_EQ((*rows[2])[1], Value::from_bool(true));
END_TEST

bool cmp_ids(const std::vector<Row*>& rows, const std::unordered_set<size_t>& ids) {
    if (rows.size() != ids.size()) {
        return false;
    }

    for (auto* row : rows) {
        if (!ids.contains(row->get_id())) {
            return false;
        }
    }

    return true;
}

TEST(filter, table)
    auto table = get_filled_table();

    ASSERT(cmp_ids(
        table.get_filtered_rows(BinaryExpr(
            BinaryExprOp::EQ,
            std::make_unique<Expr>(Ident("name")),
            std::make_unique<Expr>(Value::from_string("Ivan"))
        )),
        {1}
    ));

    ASSERT(cmp_ids(
        table.get_filtered_rows(BinaryExpr(
            BinaryExprOp::EQ,
            std::make_unique<Expr>(Ident("is_male")),
            std::make_unique<Expr>(Value::from_bool(true))
        )),
        {1, 3}
    ));
END_TEST
