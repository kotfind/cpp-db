add_library(cpp-db STATIC)

target_sources(cpp-db
    PUBLIC
        Column.hpp
        Ident.hpp
        Row.hpp
        RowInitializerNamed.hpp
        RowInitializerPositioned.hpp
        Table.hpp
        Value.hpp
        Expr.hpp

    PRIVATE
        Column.cpp
        Ident.cpp
        Row.cpp
        RowInitializerNamed.cpp
        RowInitializerPositioned.cpp
        Table.cpp
        Value.cpp
        Expr.cpp
)
