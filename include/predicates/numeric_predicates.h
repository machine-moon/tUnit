#pragma once

#include <functional>
#include <type_traits>

namespace tunit {
namespace predicates {

// ==================== Numeric Predicates ====================

template <typename T>
constexpr bool is_integral_type_v = std::is_integral_v<std::decay_t<T>>;

struct is_even {
  template <typename T>
  constexpr bool operator()(const T& value) const noexcept {
    static_assert(is_integral_type_v<T>, "is_even requires an integral type");
    return value % 2 == 0;
  }

  template <typename T, typename U>
  constexpr bool operator()(const T& lhs, const U& rhs) const noexcept {
    static_assert(is_integral_type_v<T>, "is_even requires an integral type");
    static_assert(is_integral_type_v<U>, "is_even requires an integral type");
    return (*this)(lhs) && (*this)(rhs);
  }
};

struct is_odd {
  template <typename T>
  constexpr bool operator()(const T& value) const noexcept {
    static_assert(is_integral_type_v<T>, "is_odd requires an integral type");
    return !is_even{}(value);
  }

  template <typename T, typename U>
  constexpr bool operator()(const T& lhs, const U& rhs) const noexcept {
    static_assert(is_integral_type_v<T>, "is_odd requires an integral type");
    static_assert(is_integral_type_v<U>, "is_odd requires an integral type");
    return (*this)(lhs) && (*this)(rhs);
  }
};

struct is_positive {
  template <typename T>
  constexpr bool operator()(const T& value) const noexcept {
    return std::greater<T>{}(value, T{});
  }

  template <typename T, typename U>
  constexpr bool operator()(const T& lhs, const U& rhs) const noexcept {
    return (*this)(lhs) && (*this)(rhs);
  }
};

struct is_negative {
  template <typename T>
  constexpr bool operator()(const T& value) const noexcept {
    return std::less<T>{}(value, T{});
  }

  template <typename T, typename U>
  constexpr bool operator()(const T& lhs, const U& rhs) const noexcept {
    return (*this)(lhs) && (*this)(rhs);
  }
};

struct is_zero {
  template <typename T>
  constexpr bool operator()(const T& value) const noexcept {
    return std::equal_to<T>{}(value, T{});
  }

  template <typename T, typename U>
  constexpr bool operator()(const T& lhs, const U& rhs) const noexcept {
    return std::equal_to<T>{}(lhs, T{}) && std::equal_to<U>{}(rhs, U{});
  }
};

}  // namespace predicates
}  // namespace tunit
