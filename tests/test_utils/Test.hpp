#pragma once

#include <functional>
#include <optional>
#include <string>

class TestGroup;
class TestError;

class Test {
    public:
        Test(std::string name, TestGroup* group, std::function<bool()> body);

        const std::string& get_name() const;
        std::string get_full_name() const;
        TestGroup* get_group() const;

        /// Returns TestError on error and None on success
        std::optional<TestError> run() const;

    private:
        std::optional<TestError> run_inner() const;

        std::string name;
        TestGroup* group;
        std::function<bool()> body;
};
