#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <string>

#include "../predicate_config.h"

namespace tunit::predicates {

/**
 * @brief Tests if a number is a perfect square
 */
struct is_perfect_square {
  TUNIT_CONSTEXPR is_perfect_square() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(T value) const {
    TUNIT_TRACE_PREDICATE("is_perfect_square");
    if (value < 0) return false;
    int root = static_cast<int>(std::sqrt(value));
    return root * root == value;
  }
};

/**
 * @brief Tests if a string is a palindrome (case and non-alphanumeric insensitive)
 */
struct is_palindrome {
  TUNIT_CONSTEXPR is_palindrome() noexcept = default;

  bool operator()(const std::string& str) const {
    TUNIT_TRACE_PREDICATE("is_palindrome");
    std::string clean_str;
    for (char c : str) {
      if (std::isalnum(c)) {
        clean_str += std::tolower(c);
      }
    }
    std::string reversed = clean_str;
    std::reverse(reversed.begin(), reversed.end());
    return clean_str == reversed;
  }
};

}  // namespace tunit::predicates
