#pragma once

#include <vector>
#include <memory>

namespace test_utils {
    class TestGroup;
    class TestError;
    class Test;

    class Tester {
        public:
            static Tester* get();

            void run();

            void print_stats() const;

            TestGroup* get_group() const;

        private:
            Tester();

            /// The top-level group
            std::unique_ptr<TestGroup> group;
            std::vector<TestError> failed;
    };
}
