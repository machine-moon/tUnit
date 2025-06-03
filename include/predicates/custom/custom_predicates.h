#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <string>

#include "predicates/predicate_config.h"

namespace tUnit::predicates
{

/**
 * Tests if a number is a perfect square
 */
struct is_perfect_square
{
  TUNIT_CONSTEXPR is_perfect_square() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(T value) const
  {
    TUNIT_TRACE_PREDICATE("is_perfect_square");
    if (value < 0) return false;
    std::int64_t root = static_cast<std::int64_t>(std::sqrt(value));
    return root * root == value;
  }
};

/**
 * Tests if a string is a palindrome
 */
struct is_palindrome
{
  TUNIT_CONSTEXPR is_palindrome() noexcept = default;

  bool operator()(const std::string &str) const
  {
    TUNIT_TRACE_PREDICATE("is_palindrome");
    std::string clean_str;
    for (char c : str)
    {
      if (std::isalnum(c))
      {
        clean_str += std::tolower(c);
      }
    }
    std::string reversed = clean_str;
    std::reverse(reversed.begin(), reversed.end());
    return clean_str == reversed;
  }
};

/**
 * Tests if a number is prime
 */
struct is_prime
{
  TUNIT_CONSTEXPR is_prime() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(T value) const
  {
    TUNIT_TRACE_PREDICATE("is_prime");
    if (value < 2) return false;
    if (value == 2) return true;
    if (value % 2 == 0) return false;
    for (T i = 3; i * i <= value; i += 2)
    {
      if (value % i == 0) return false;
    }
    return true;
  }
};

} // namespace tUnit::predicates
