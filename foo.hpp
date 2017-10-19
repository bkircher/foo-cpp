#pragma once

#include <algorithm>
#include <string>

namespace foo {
inline int multiply(int a, int b) { return a * b; }

inline std::string uppercase(std::string str) {
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
}
}
