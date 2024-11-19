#include "TestError.hpp"

TestError::TestError(const Test* test, std::string msg)
  : test(test),
    msg(std::move(msg))
{}

const std::string& TestError::get_msg() const {
    return msg;
}

const Test* TestError::get_test() const {
    return test;
}
