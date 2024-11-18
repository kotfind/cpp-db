#include "Database.hpp"
#include "Column.hpp"
#include "Table.hpp"
#include "macro.hpp"
#include "test_utils.hpp"

TEST_GROUP(database)

TEST(create_table, database)
    Database db;

    db.create_table_query({
        .table_name = Ident("people"),
        .columns = {
            Column(Ident("name"), ValueType::STRING),
            Column(Ident("is_male"), ValueType::BOOL),
        }
    });

    auto table = db.get_table(Ident("people"));

    ASSERT_EQ(table->get_name(), Ident("people"));
    ASSERT_EQ(table->get_column(0), Column(Ident("name"), ValueType::STRING));
    ASSERT_EQ(table->get_column(1), Column(Ident("is_male"), ValueType::BOOL));
END_TEST

TEST(complex, database)
    Database db;

    db.create_table_query({
        .table_name = Ident("people"),
        .columns = {
            Column(Ident("name"), ValueType::STRING),
            Column(Ident("is_male"), ValueType::BOOL),
        }
    });

    db.insert_query({
        .row = {RowInitializerPositioned {{ Value::from_string("Ivan"), Value::from_bool(true) }}},
        .table_name = Ident("people")
    });

    db.insert_query({
        .row = {RowInitializerPositioned {{ Value::from_string("Ann"), Value::from_bool(false) }}},
        .table_name = Ident("people")
    });

    db.insert_query({
        .row = {RowInitializerPositioned {{ Value::from_string("Jack"), Value::from_bool(true) }}},
        .table_name = Ident("people")
    });

    db.update_query({
        .table_name = Ident("people"),
        .assignments = {
            {Ident("name"), Value::from_string("James")}
        },
        .cond = Ident("name") == Value::from_string("Jack"),
    });

    {
        auto resp = db.select_query({
            .table_name = Ident("people"),
            .cond = Ident("name") == Value::from_string("James")
        });

        ASSERT_EQ(resp.size(), 1);
        ASSERT_EQ(resp[0]->get_id(), 3);
    }

    {
        auto resp = db.select_query({
            .table_name = Ident("people"),
            .cond = len(Ident("name")) == Value::from_int(3)
        });

        ASSERT_EQ(resp.size(), 1);
        ASSERT_EQ(resp[0]->get_id(), 2);
    }

    auto resp = db.delete_query({
        .table_name = Ident("people"),
        .cond = Ident("name") == Value::from_string("James")
    });

    {
        auto resp = db.select_query({
            .table_name = Ident("people"),
            .cond = Ident("name") == Value::from_string("James")
        });

        ASSERT_EQ(resp.size(), 0);
    }
END_TEST
