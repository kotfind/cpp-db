cmake_minimum_required(VERSION 3.23)
project(cpp-db LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

find_program(MOLD_EXECUTABLE mold)
if (NOT ${MOLD_EXECUTABLE} STREQUAL MOLD_EXECUTABLE-NOTFOUND)
    set(CMAKE_CXX_FLAGS "-fuse-ld=mold")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-fdiagnostics-color=always -fsanitize=undefined,address,float-divide-by-zero,float-cast-overflow -g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -O2")

add_executable(cpp-db
    main.cpp
)

add_custom_target(run COMMAND cpp-db)
