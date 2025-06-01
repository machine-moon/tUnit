// NOTE: These logical combinators are included because they can be useful in generic code.
// However, when using predicate functors, it often makes sense to use the built-in operators (||, &&, !) directly for better readability.
#pragma once

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

#include "../predicate_config.h"

namespace tunit {
namespace predicates {

/**
 * @brief Logical AND combinator for predicates
 */
template <typename T, typename U>
struct and_ {
  T lhs_;
  U rhs_;

  TUNIT_CONSTEXPR and_(T&& lhs, U&& rhs) noexcept : lhs_(std::forward<T>(lhs)), rhs_(std::forward<U>(rhs)) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args&&... args) const {
    TUNIT_TRACE_PREDICATE("and_");
    return lhs_(args...) && rhs_(args...);
  }
};

/**
 * @brief Logical OR combinator for predicates
 */
template <typename T, typename U>
struct or_ {
  T lhs_;
  U rhs_;

  TUNIT_CONSTEXPR or_(T&& lhs, U&& rhs) noexcept : lhs_(std::forward<T>(lhs)), rhs_(std::forward<U>(rhs)) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args&&... args) const {
    TUNIT_TRACE_PREDICATE("or_");
    return lhs_(args...) || rhs_(args...);
  }
};

/**
 * @brief Logical XOR combinator for predicates
 */
template <typename T, typename U>
struct xor_ {
  T lhs_;
  U rhs_;

  TUNIT_CONSTEXPR xor_(T&& lhs, U&& rhs) noexcept : lhs_(std::forward<T>(lhs)), rhs_(std::forward<U>(rhs)) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args&&... args) const {
    TUNIT_TRACE_PREDICATE("xor_");
    const bool lhs_result = lhs_(args...);
    const bool rhs_result = rhs_(args...);
    return lhs_result != rhs_result;
  }
};

/**
 * @brief Logical NOT combinator for predicates
 */
template <typename P>
struct not_ {
  P pred_;

  TUNIT_CONSTEXPR not_(P&& pred) noexcept : pred_(std::forward<P>(pred)) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args&&... args) const {
    TUNIT_TRACE_PREDICATE("not_");
    return !pred_(args...);
  }
};

/**
 * @brief Logical NAND combinator for predicates
 */
template <typename T, typename U>
struct nand_ {
  T lhs_;
  U rhs_;

  TUNIT_CONSTEXPR nand_(T&& lhs, U&& rhs) noexcept : lhs_(std::forward<T>(lhs)), rhs_(std::forward<U>(rhs)) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args&&... args) const {
    TUNIT_TRACE_PREDICATE("nand_");
    return !(lhs_(args...) && rhs_(args...));
  }
};

/**
 * @brief Logical NOR combinator for predicates
 */
template <typename T, typename U>
struct nor_ {
  T lhs_;
  U rhs_;

  TUNIT_CONSTEXPR nor_(T&& lhs, U&& rhs) noexcept : lhs_(std::forward<T>(lhs)), rhs_(std::forward<U>(rhs)) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args&&... args) const {
    TUNIT_TRACE_PREDICATE("nor_");
    return !(lhs_(args...) || rhs_(args...));
  }
};

/**
 * @brief Logical implication combinator for predicates (A implies B = !A || B)
 */
template <typename T, typename U>
struct implies {
  T condition_;
  U result_;

  TUNIT_CONSTEXPR implies(T&& condition, U&& result) noexcept : condition_(std::forward<T>(condition)), result_(std::forward<U>(result)) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args&&... args) const {
    TUNIT_TRACE_PREDICATE("implies");
    return !condition_(args...) || result_(args...);
  }
};

}  // namespace predicates
}  // namespace tunit
