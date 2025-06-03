#pragma once

#include <functional>

#include "predicates/predicate_config.h"

namespace tunit
{
namespace predicates
{

/**
 * Tests if a value is within a specified range (inclusive)
 */
struct is_in_range
{
  TUNIT_CONSTEXPR is_in_range() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T &value, const T &min_val, const T &max_val) const
  {
    TUNIT_TRACE_PREDICATE("is_in_range");
    return !std::less<T>{}(value, min_val) && !std::less<T>{}(max_val, value);
  }
};

/**
 * Tests if a value is outside a specified range
 */
struct is_out_of_range
{
  TUNIT_CONSTEXPR is_out_of_range() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T &value, const T &min_val, const T &max_val) const
  {
    TUNIT_TRACE_PREDICATE("is_out_of_range");
    return !is_in_range{}(value, min_val, max_val);
  }
};

} // namespace predicates
} // namespace tunit
