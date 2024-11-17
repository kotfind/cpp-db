add_library(test_utils STATIC)

target_sources(test_utils
    PUBLIC
        Test.hpp
        TestGroup.hpp
        Tester.hpp
        macro.hpp

    PRIVATE
        Test.cpp
        TestError.cpp
        TestGroup.cpp
        Tester.cpp
)
