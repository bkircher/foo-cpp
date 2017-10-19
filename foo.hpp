#pragma once

#include <algorithm>
#include <cctype>
#include <string>

namespace foo {
inline int multiply(int a, int b) { return a * b; }

inline std::string uppercase(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
  return s;
}
}
