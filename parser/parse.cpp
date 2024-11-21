#include <string_view>
#include <string>

constexpr size_t UNPARSED_TAIL_MAX_LEN = 50;

namespace parser::__impl {
    std::string form_unparsed_tail(std::string_view tail) {
        if (tail.size() <= UNPARSED_TAIL_MAX_LEN) {
            return std::string(tail);
        } else {
            return std::string(tail.substr(0, UNPARSED_TAIL_MAX_LEN - 3)) + "...";
        }
    }
}
