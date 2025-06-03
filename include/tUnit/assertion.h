#pragma once
#include <string>

namespace tUnit
{

/**
 * Represents a single test assertion with description and result
 */
class Assertion
{
public:
  std::string description_;
  bool result_;

  Assertion(const std::string &desc, bool result) : description_(desc), result_(result) {}
};

} // namespace tUnit