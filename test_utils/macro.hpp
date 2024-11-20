#pragma once

#include <sstream>
#include <exception>

/// Macro from https://stackoverflow.com/a/21371401
#define GET_MACRO(_0, _1, _2, NAME, ...) NAME

#define ERROR #error wrong number of arguments

#define CONCAT_(x, y) x ## y
#define CONCAT(x, y) CONCAT_(x, y)

// -------------------- Group --------------------

#define TEST_GROUP_NAME(name) CONCAT(__test_group_, name)

#define TEST_GROUP(...) GET_MACRO(_0, ##__VA_ARGS__, __TEST_GROUP_2, __TEST_GROUP_1, ARG_NUM_ERROR)(__VA_ARGS__)

#define __TEST_GROUP_1(name) \
    test_utils::TestGroup TEST_GROUP_NAME(name)(#name);

#define __TEST_GROUP_2(name, parent) \
    test_utils::TestGroup TEST_GROUP_NAME(name)(#name, &TEST_GROUP_NAME(parent));

// -------------------- Test --------------------

#define TEST_NAME(name) CONCAT(__test_, name)

#define TEST(...) GET_MACRO(_0, ##__VA_ARGS__, __TEST_2, __TEST_1, ARG_NUM_ERROR)(__VA_ARGS__)

#define __TEST_1(name) \
    static test_utils::Test TEST_NAME(name)(#name, nullptr, []() {

#define __TEST_2(name, group) \
    static test_utils::Test CONCAT(TEST_GROUP_NAME(group), TEST_NAME(name))(#name, &TEST_GROUP_NAME(group), []() {

#define END_TEST \
        return true; \
    });

// -------------------- Assert --------------------

#define ASSERT(x) \
    if (!(x)) { \
        std::stringstream ss; \
        ss  << "ASSERT in " << __FILE__ << ":" << __LINE__ << " failed:\n" \
            << #x; \
        throw std::runtime_error(ss.str()); \
    }

#define ASSERT_EQ(lhs, rhs) \
    if ((lhs) != (rhs)) { \
        std::stringstream ss; \
        ss  << "ASSERT_EQ in " << __FILE__ << ":" << __LINE__ << " failed:\n" \
            << "    lhs: " << #lhs << "\n" \
            << "    rhs: " << #rhs; \
        throw std::runtime_error(ss.str()); \
    }

// -------------------- Main --------------------

#define TESTING_MAIN \
    int main() { \
        auto* tester = test_utils::Tester::get(); \
        tester->run(); \
        tester->print_stats(); \
    }
