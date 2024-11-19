set(TARGET test_utils)

add_library(${TARGET} STATIC)

target_sources(${TARGET}
    PRIVATE
        Test.cpp
        TestError.cpp
        TestGroup.cpp
        Tester.cpp
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
