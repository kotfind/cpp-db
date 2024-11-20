set(TARGET db-tests)

add_executable(${TARGET})

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "*.cpp")

target_sources(${TARGET} PRIVATE ${SRC})

target_link_libraries(${TARGET} PRIVATE test_utils db parser)

include_directories(..)

add_custom_target(db-test COMMAND ${TARGET})
