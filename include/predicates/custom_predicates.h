#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <string>

namespace tunit::predicates {

// Custom predicate to check if a number is a perfect square
struct is_perfect_square {
  template <typename T>
  bool operator()(T value) const {
    if (value < 0) return false;
    int root = static_cast<int>(std::sqrt(value));
    return root * root == value;
  }
};

// Custom predicate to check if a string is a palindrome
struct is_palindrome {
  bool operator()(const std::string& str) const {
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
