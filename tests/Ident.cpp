#include "test_utils.hpp"

#include <functional>
#include "Ident.hpp"

TEST_GROUP(ident)

TEST(get_inner, ident)
    ASSERT_EQ(Ident("some_name_123").get_inner(), "some_name_123");
END_TEST

TEST(eq, ident)
    ASSERT(Ident("n") == Ident("n"));
    ASSERT(Ident("n") != Ident("m"));
END_TEST

TEST(hash, ident)
    ASSERT(std::hash<Ident>()(Ident("n")) == std::hash<Ident>()(Ident("n")));
    ASSERT(std::hash<Ident>()(Ident("n")) != std::hash<Ident>()(Ident("m")));
END_TEST
