#pragma once

#include <functional>

#include "../predicate_config.h"

namespace tunit {
namespace predicates {

/**
 * Tests if two values are equal
 */
struct is_equal {
  TUNIT_CONSTEXPR is_equal() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T& lhs, const U& rhs) const {
    TUNIT_TRACE_PREDICATE("is_equal");
    return lhs == rhs;
  }
};

/**
 * Tests if two values are not equal
 */
struct is_not_equal {
  TUNIT_CONSTEXPR is_not_equal() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T& lhs, const U& rhs) const {
    TUNIT_TRACE_PREDICATE("is_not_equal");
    return lhs != rhs;
  }
};

/**
 * Tests if the first value is greater than the second
 */
struct is_greater {
  TUNIT_CONSTEXPR is_greater() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T& lhs, const U& rhs) const {
    TUNIT_TRACE_PREDICATE("is_greater");
    return lhs > rhs;
  }
};

/**
 * Tests if the first value is less than the second
 */
struct is_less {
  TUNIT_CONSTEXPR is_less() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T& lhs, const U& rhs) const {
    TUNIT_TRACE_PREDICATE("is_less");
    return lhs < rhs;
  }
};

/**
 * Tests if the first value is greater than or equal to the second
 */
struct is_greater_equal {
  TUNIT_CONSTEXPR is_greater_equal() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T& lhs, const U& rhs) const {
    TUNIT_TRACE_PREDICATE("is_greater_equal");
    return lhs >= rhs;
  }
};

/**
 * Tests if the first value is less than or equal to the second
 */
struct is_less_equal {
  TUNIT_CONSTEXPR is_less_equal() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T& lhs, const U& rhs) const {
    TUNIT_TRACE_PREDICATE("is_less_equal");
    return lhs <= rhs;
  }
};

}  // namespace predicates
}  // namespace tunit
