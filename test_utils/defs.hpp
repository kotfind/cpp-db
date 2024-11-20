#pragma once

#include <cstddef>
#include <string>

namespace test_utils {
    constexpr size_t TEST_MSG_WIDTH = 80;

    constexpr std::string OK_MSG = "OK";
    constexpr std::string FAILED_MSG = "FAILED";

    constexpr std::string CLEAR = "\033[0m";
    constexpr std::string RED = "\033[31m";
    constexpr std::string GREEN = "\033[32m";
}
