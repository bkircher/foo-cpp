#include <string>
#include <tuple>
#include <vector>

#include <gtest/gtest.h>

#include "foo.hpp"

using namespace testing;

class FooTest : public TestWithParam<std::tuple<std::string, std::string>> {
protected:
  virtual void SetUp() { std::tie(first, second) = GetParam(); }

  virtual void TearDown() {}

  std::string first;
  std::string second;
};

TEST(FooTest, MultiplySomething) { EXPECT_EQ(22, foo::multiply(2, 11)); }

TEST_P(FooTest, UppercaseSomething) { EXPECT_STREQ(foo::uppercase(first).c_str(), second.c_str()); }

static auto test_values = std::vector<std::tuple<std::string, std::string>>{
    {"a", "A"}, {"c", "C"}, {"e", "E"},
};

INSTANTIATE_TEST_CASE_P(MeaningfulTestParameters, FooTest, ValuesIn(test_values));
