#include <string>
#include <tuple>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch-wrapper.hpp"

#include "foo.hpp"

TEST_CASE("Multiplication works", "[multiply]") { REQUIRE(foo::multiply(2, 11) == 22); }

TEST_CASE("uppercase converts entire strings", "[uppercase]") {
  const auto test_values = std::vector<std::tuple<std::string, std::string>>{
      {"a", "A"}, {"c", "C"}, {"e", "E"},
  };

  SECTION("most common values") {
    std::string first;
    std::string second;

    for (const auto& val : test_values) {
      std::tie(first, second) = val;
      REQUIRE(foo::uppercase(first) == second);
    }
  }
}
