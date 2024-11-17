add_library(cpp-db STATIC)

target_sources(cpp-db
    PUBLIC
        mul.hpp
    PRIVATE
        mul.cpp
)
