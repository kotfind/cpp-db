
add_executable(tests)

target_sources(tests PRIVATE
    main.cpp
    Value.cpp
)

add_subdirectory(test_utils)
target_include_directories(tests PRIVATE test_utils)
target_link_libraries(tests PRIVATE test_utils cpp-db)

add_custom_target(test COMMAND tests)
