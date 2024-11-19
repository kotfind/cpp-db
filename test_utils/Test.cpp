#include "Test.hpp"

#include "TestGroup.hpp"
#include "TestError.hpp"
#include "Tester.hpp"
#include "defs.hpp"

#include <iostream>
#include <optional>
#include <sstream>
#include <string>

Test::Test(std::string name, TestGroup *group, std::function<void()> body)
    : name(name), body(body) {
  if (group) {
    this->group = group;
  } else {
    this->group = Tester::get()->get_group();
  }
  this->group->push_test(this);
}

const std::string& Test::get_name() const {
    return name;
}

std::string Test::get_full_name() const {
    auto group_name = group->get_full_name();
    if (group_name.empty()) {
        return name;
    } else {
        return group_name + "::" + name;
    }
}

TestGroup* Test::get_group() const {
    return group;
}

std::optional<TestError> Test::run() const {
    auto result = run_inner();

    size_t str_result_len;
    if (result.has_value()) {
        str_result_len = FAILED_MSG.length();
    } else {
        str_result_len = OK_MSG.length();
    }

    auto full_name = get_full_name();
    std::string dots;

    std::cout << full_name << ' ';

    for (size_t i = str_result_len + full_name.length() + 2; i < TEST_MSG_WIDTH; ++i) {
        std::cout << '.';
    }

    std::cout << ' ';
    if (result.has_value()) {
        std::cout << RED << FAILED_MSG << CLEAR << std::endl;
    } else {
        std::cout << GREEN << OK_MSG << CLEAR << std::endl;
    }

    return result;
}

std::optional<TestError> Test::run_inner() const {
    try {
        body();
        return std::optional<TestError>();
    } catch (const std::exception &e) {
        std::stringstream ss;
        ss  << "test failed with exception:\n"
            << e.what();
        return std::make_optional<TestError>(this, ss.str());
    } catch (...) {
        return std::make_optional<TestError>(this, "test failed with unknown exception");
    }
}

