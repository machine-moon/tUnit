// NOTE: These logical combinators are included because they can be useful in generic code.
//  However, when using predicate functors, it often makes sense to use the built-in operators (||, &&, !) directly for better readability.
#pragma once

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

namespace tunit {
namespace predicates {

// ==================== Logical Combinators ====================

template <typename T, typename U>
struct and_ {
  T lhs_;
  U rhs_;

  constexpr and_(T&& lhs, U&& rhs) noexcept : lhs_(std::forward<T>(lhs)), rhs_(std::forward<U>(rhs)) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return lhs_(args...) && rhs_(args...);
  }
};

template <typename T, typename U>
struct or_ {
  T lhs_;
  U rhs_;

  constexpr or_(T&& lhs, U&& rhs) noexcept : lhs_(std::forward<T>(lhs)), rhs_(std::forward<U>(rhs)) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return lhs_(args...) || rhs_(args...);
  }
};

template <typename T, typename U>
struct xor_ {
  T lhs_;
  U rhs_;

  constexpr xor_(T&& lhs, U&& rhs) noexcept : lhs_(std::forward<T>(lhs)), rhs_(std::forward<U>(rhs)) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    const bool lhs_result = lhs_(args...);
    const bool rhs_result = rhs_(args...);
    return lhs_result != rhs_result;
  }
};

template <typename P>
struct not_ {
  P pred_;

  constexpr explicit not_(P&& pred) noexcept : pred_(std::forward<P>(pred)) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return !pred_(args...);
  }
};

template <typename T, typename U>
struct nand_ {
  T lhs_;
  U rhs_;

  constexpr nand_(T&& lhs, U&& rhs) noexcept : lhs_(std::forward<T>(lhs)), rhs_(std::forward<U>(rhs)) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return !(lhs_(args...) && rhs_(args...));
  }
};

template <typename T, typename U>
struct nor_ {
  T lhs_;
  U rhs_;

  constexpr nor_(T&& lhs, U&& rhs) noexcept : lhs_(std::forward<T>(lhs)), rhs_(std::forward<U>(rhs)) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return !(lhs_(args...) || rhs_(args...));
  }
};

// (A implies B = !A || B)
template <typename T, typename U>
struct implies {
  T condition_;
  U result_;

  constexpr implies(T&& condition, U&& result) noexcept : condition_(std::forward<T>(condition)), result_(std::forward<U>(result)) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return !condition_(args...) || result_(args...);
  }
};

}  // namespace predicates
}  // namespace tunit
