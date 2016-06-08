#include <gtest/gtest.h>

#include <foo.hpp>

TEST(TestFoo, MultiplyTwoIntegers) { EXPECT_EQ(56, foo::multiply(7, 8)); }

TEST(TestFoo, SomethingShouldWork) { FAIL() << "Get to work!"; }

// vim:et ts=4 sw=4 noic cc=80
