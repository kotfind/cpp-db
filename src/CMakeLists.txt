add_library(cpp-db STATIC)

target_sources(cpp-db
    PUBLIC
        Value.hpp
    PRIVATE
        Value.cpp
)
