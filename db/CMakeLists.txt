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
        parsing.cpp
        TempColumn.cpp
        TempRow.cpp
        TempTable.cpp
)

target_include_directories(${TARGET}
    PRIVATE
        .
    PUBLIC
        include
)

set_target_properties(${TARGET} PROPERTIES PUBLIC_HEADER
    include/db.hpp
)

target_link_libraries(${TARGET} PRIVATE parser)

add_subdirectory(tests)
