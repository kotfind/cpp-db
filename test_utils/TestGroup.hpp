#pragma once

#include <string>
#include <vector>

namespace test_utils {
    class Test;
    class TestError;
    class Tester;

    class TestGroup {
        public:
            TestGroup(std::string name, TestGroup* parent_group = nullptr);

            const std::string& get_name() const;
            std::string get_full_name() const;
            TestGroup* get_parent_group() const;

            std::vector<TestError> run_tests() const;

        private:
            /// For constructing top-level group in Testser
            TestGroup();

            void push_test(Test* test);

            void push_group(TestGroup* group);

            std::string name;
            TestGroup* parent_group;
            std::vector<Test*> tests;
            std::vector<TestGroup*> groups;

        friend Test;
        friend Tester;
    };
}
