#include "Tester.hpp"

#include "TestGroup.hpp"
#include "Test.hpp"
#include "TestError.hpp"
#include "defs.hpp"

#include <iostream>
#include <memory>
#include <ostream>

namespace test_utils {
    Tester* Tester::get() {
        static Tester* instance = new Tester;
        return instance;
    }

    TestGroup* Tester::get_group() const {
        return group.get();
    }

    void Tester::run() {
        std::cout << "Running tests...\n" << std::endl;

        failed = group->run_tests();
    }

    void Tester::print_stats() const {
        std::cout << '\n';

        if (failed.empty()) {
            std::cout << GREEN << "All tests passed!" << CLEAR << std::endl;
            return;
        }

        std::cout << RED << failed.size() << " tests failed!" << CLEAR << std::endl;

        for (auto error : failed) {
            std::cout << "\n";

            auto title = error.get_test()->get_full_name() + " failed";
            size_t padding = ((int)TEST_MSG_WIDTH - title.length() - 2) / 2 - 1;
            for (int i = 0; i < padding; ++i) {
                std::cout << '-';
            }
            std::cout << ' ' << title << ' ';
            for (int i = 0; i < padding; ++i) {
                std::cout << '-';
            }

            std::cout << '\n' << error.get_msg() << std::endl;
        }
        std::cout << '\n';
    }

    Tester::Tester()
      : group(std::unique_ptr<TestGroup>(new TestGroup)),
        failed()
    {}
}
