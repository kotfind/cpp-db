set(TARGET db)

add_library(${TARGET} STATIC)

target_sources(${TARGET}
    PRIVATE
        Column.cpp
        Ident.cpp
        Row.cpp
        RowInitializerNamed.cpp
        RowInitializerPositioned.cpp
        Table.cpp
        Value.cpp
        Expr.cpp
        Database.cpp
)

target_include_directories(${TARGET}
    PRIVATE
        .
    PUBLIC
        include
)

set_target_properties(${TARGET} PROPERTIES PUBLIC_HEADER
    include/test_utils.hpp
)

add_subdirectory(tests)
