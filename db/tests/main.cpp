#include "test_utils.hpp"

int main() {
    auto* tester = Tester::get();
    tester->run();
    tester->print_stats();
}
