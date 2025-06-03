#pragma once
#include <string>

namespace tunit {

struct Assertion {
  std::string description_;
  bool result_;

  Assertion(const std::string &desc, bool result)
      : description_(desc), result_(result) {}
};

} // namespace tunit