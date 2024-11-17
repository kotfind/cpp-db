add_executable(tests)

target_sources(tests
    PUBLIC
        test_utils.hpp

    PRIVATE
        Test.cpp
        TestError.cpp
        TestGroup.cpp
        Tester.cpp
        main.cpp
)

add_custom_target(test COMMAND tests)
