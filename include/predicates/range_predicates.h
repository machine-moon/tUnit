#pragma once

#include <functional>

namespace tunit {
namespace predicates {

// ==================== Range Predicates ====================

struct is_in_range {
  template <typename T>
  constexpr bool operator()(const T& value, const T& min_val, const T& max_val) const noexcept {
    return !std::less<T>{}(value, min_val) && !std::less<T>{}(max_val, value);
  }
};

struct is_out_of_range {
  template <typename T>
  constexpr bool operator()(const T& value, const T& min_val, const T& max_val) const noexcept {
    return !is_in_range{}(value, min_val, max_val);
  }
};

}  // namespace predicates
}  // namespace tunit
