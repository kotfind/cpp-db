#include "Tester.hpp"

int main() {
    auto* tester = Tester::get();

    tester->run();

    tester->print_stats();
}
