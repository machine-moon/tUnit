/**
 * Numeric predicates for mathematical comparisons and validations
 */
#pragma once

#include <functional>
#include <type_traits>

#include "predicates/predicate_config.h"

namespace tUnit
{
namespace predicates
{

template <typename T>
constexpr bool is_integral_type_v = std::is_integral_v<std::decay_t<T>>;

/**
 * Tests if a number is even
 */
struct is_even
{
  TUNIT_CONSTEXPR is_even() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T &value) const
  {
    TUNIT_TRACE_PREDICATE("is_even");
    static_assert(is_integral_type_v<T>, "is_even requires an integral type");
    return value % 2 == 0;
  }

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T &lhs, const U &rhs) const
  {
    TUNIT_TRACE_PREDICATE("is_even (binary)");
    static_assert(is_integral_type_v<T>, "is_even requires an integral type");
    static_assert(is_integral_type_v<U>, "is_even requires an integral type");
    return (*this)(lhs) && (*this)(rhs);
  }
};

/**
 * Tests if a number is odd
 */
struct is_odd
{
  TUNIT_CONSTEXPR is_odd() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T &value) const
  {
    TUNIT_TRACE_PREDICATE("is_odd");
    static_assert(is_integral_type_v<T>, "is_odd requires an integral type");
    return !is_even{}(value);
  }

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T &lhs, const U &rhs) const
  {
    TUNIT_TRACE_PREDICATE("is_odd (binary)");
    static_assert(is_integral_type_v<T>, "is_odd requires an integral type");
    static_assert(is_integral_type_v<U>, "is_odd requires an integral type");
    return (*this)(lhs) && (*this)(rhs);
  }
};

/**
 * Tests if a number is positive
 */
struct is_positive
{
  TUNIT_CONSTEXPR is_positive() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T &value) const
  {
    TUNIT_TRACE_PREDICATE("is_positive");
    return std::greater<T>{}(value, T{});
  }

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T &lhs, const U &rhs) const
  {
    TUNIT_TRACE_PREDICATE("is_positive (binary)");
    return (*this)(lhs) && (*this)(rhs);
  }
};

/**
 * Tests if a number is negative
 */
struct is_negative
{
  TUNIT_CONSTEXPR is_negative() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T &value) const
  {
    TUNIT_TRACE_PREDICATE("is_negative");
    return std::less<T>{}(value, T{});
  }

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T &lhs, const U &rhs) const
  {
    TUNIT_TRACE_PREDICATE("is_negative (binary)");
    return (*this)(lhs) && (*this)(rhs);
  }
};

/**
 * Tests if a number is zero
 */
struct is_zero
{
  TUNIT_CONSTEXPR is_zero() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T &value) const
  {
    TUNIT_TRACE_PREDICATE("is_zero");
    return std::equal_to<T>{}(value, T{});
  }

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T &lhs, const U &rhs) const
  {
    TUNIT_TRACE_PREDICATE("is_zero (binary)");
    return std::equal_to<T>{}(lhs, T{}) && std::equal_to<U>{}(rhs, U{});
  }
};

} // namespace predicates
} // namespace tUnit
