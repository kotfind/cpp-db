add_library(cpp-db STATIC)

target_sources(cpp-db
    PUBLIC
        Value.hpp
        Column.hpp
        Ident.hpp
        Row.hpp

    PRIVATE
        Value.cpp
        Column.cpp
        Ident.cpp
        Row.cpp
)
