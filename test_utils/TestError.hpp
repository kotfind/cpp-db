#pragma once

#include <string>

class Test;

class TestError {
    public:
        TestError(const Test* test, std::string msg);

        const std::string& get_msg() const;
        const Test* get_test() const;

    private:
        std::string msg;
        const Test* test;
};