#include "TestGroup.hpp"

#include "Tester.hpp"
#include "Test.hpp"
#include "TestError.hpp"

TestGroup::TestGroup(std::string name, TestGroup* parent_group)
  : name(name),
    tests()
{
    if (parent_group) {
        this->parent_group = parent_group;
    } else {
        this->parent_group = Tester::get()->get_group();
    }
    this->parent_group->push_group(this);
}

TestGroup::TestGroup()
  : name(""),
    parent_group(nullptr),
    tests()
{}

const std::string& TestGroup::get_name() const {
    return name;
}

std::string TestGroup::get_full_name() const {
    auto ans = std::string("");

    if (parent_group) {
        auto parent_group_name = parent_group->get_full_name();
        if (!parent_group_name.empty()) {
            ans += parent_group_name + "::";
        }
    }

    ans += name;

    return ans;
}

TestGroup* TestGroup::get_parent_group() const {
    return parent_group;
}

std::vector<TestError> TestGroup::run_tests() const {
    std::vector<TestError> ans;

    for (auto* group : groups) {
        auto group_ans = group->run_tests();
        ans.insert(ans.end(), group_ans.begin(), group_ans.end());
    }

    for (auto* test : tests) {
        auto test_ans = test->run();
        if (test_ans) {
            ans.push_back(*test_ans);
        }
    }

    return ans;
}

void TestGroup::push_test(Test* test) {
    tests.push_back(test);
}

void TestGroup::push_group(TestGroup* group) {
    groups.push_back(group);
}
