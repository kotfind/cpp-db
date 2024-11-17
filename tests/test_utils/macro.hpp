#pragma once

/// Macro from https://stackoverflow.com/a/21371401
#define GET_MACRO(_0, _1, _2, NAME, ...) NAME

#define ERROR #error wrong number of arguments

// -------------------- Group --------------------

#define TEST_GROUP(...) GET_MACRO(_0, ##__VA_ARGS__, __TEST_GROUP_2, __TEST_GROUP_1, ARG_NUM_ERROR)(__VA_ARGS__)

#define __TEST_GROUP_1(name) \
    TestGroup name(#name);

#define __TEST_GROUP_2(name, parent) \
    TestGroup name(#name, parent);

// -------------------- Test --------------------

#define TEST(...) GET_MACRO(_0, ##__VA_ARGS__, __TEST_2, __TEST_1, ARG_NUM_ERROR)(__VA_ARGS__)

#define __TEST_1(name) \
    static Test name(#name, nullptr, []() {

#define __TEST_2(name, group) \
    static Test name(#name, &group, []() {

#define END_TEST \
        return true; \
    });

// -------------------- Assert --------------------

#define ASSERT(x) \
    if (!(x)) { \
        return false; \
    }

#define ASSERT_EQ(lhs, rhs) \
    if ((lhs) != (rhs)) { \
        return false; \
    }

// -------------------- Main --------------------
