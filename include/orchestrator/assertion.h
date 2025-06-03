#pragma once
#include <string>

namespace tunit {

struct Assertion {
  std::string description;
  bool passed;
  std::string lhs_str;
  std::string rhs_str;

  Assertion(const std::string &desc, bool pass, std::string lhs,
            std::string rhs)
      : description(desc), passed(pass), lhs_str(std::move(lhs)),
        rhs_str(std::move(rhs)) {}
};

} // namespace tunit