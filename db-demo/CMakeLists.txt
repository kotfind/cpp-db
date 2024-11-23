set(TARGET db-demo)

add_executable(${TARGET})

target_sources(${TARGET} PRIVATE main.cpp)

target_link_libraries(${TARGET} PRIVATE db)

add_custom_target(db-demo-run COMMAND ${TARGET})
