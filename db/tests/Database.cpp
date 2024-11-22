#include <test_utils.hpp>

#include "Database.hpp"
#include "Column.hpp"
#include "Table.hpp"

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

    db.delete_query({
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

TEST(complex_queries, database)
    Database db;

    db.queries(R"(
        CREATE TABLE people (
            name: str,
            is_male: bool,
        );

        INSERT ("Ivan", true) TO people;
        INSERT ("Ann", true) TO people;
        INSERT ("Jack", true) TO people;

        UPDATE people
        SET
            name = "James"
        WHERE
            name == "Jack";
    )");

    {
        auto resp = db.query(R"(
            SELECT name, is_male
            FROM people
            WHERE name == "James"
        )");

        ASSERT(resp.has_value());
        ASSERT_EQ(resp->size(), 1);
        ASSERT_EQ((*resp)[0]->get_id(), 3);
    }

    {
        auto resp = db.query(R"(
            SELECT name, is_male
            FROM people
            WHERE |name| == 3
        )");

        ASSERT(resp.has_value());
        ASSERT_EQ(resp->size(), 1);
        ASSERT_EQ((*resp)[0]->get_id(), 2);
    }

    db.query(R"(
        DELETE
        FROM people
        WHERE name == "James"
    )");

    {
        auto resp = db.query(R"(
            SELECT name, is_male
            FROM people
            WHERE name == "James"
        )");

        ASSERT(resp.has_value());
        ASSERT_EQ(resp->size(), 0);
    }

    db.query("DROP TABLE people");

    ASSERT(!db.has_table(Ident("people")));
END_TEST
