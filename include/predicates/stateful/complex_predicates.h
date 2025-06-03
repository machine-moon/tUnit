#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

#include "predicates/predicate_config.h"

namespace tunit
{
namespace predicates
{

// ********************** Complex Predicate Compositions **********************

/**
 * Tests if all provided predicates return true
 */
template <typename... P>
struct all_of
{
  std::tuple<P...> predicates_;

  template <typename... Preds>
  TUNIT_CONSTEXPR all_of(Preds &&...preds) noexcept : predicates_(std::forward<Preds>(preds)...) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args &&...args) const
  {
    TUNIT_TRACE_PREDICATE("all_of");
    return std::apply([&args...](const auto &...preds)
                      { return (preds(args...) && ...); }, predicates_);
  }
};

/**
 * Tests if any of the provided predicates return true
 */
template <typename... P>
struct any_of
{
  std::tuple<P...> predicates_;

  template <typename... Preds>
  TUNIT_CONSTEXPR any_of(Preds &&...preds) noexcept : predicates_(std::forward<Preds>(preds)...) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args &&...args) const
  {
    TUNIT_TRACE_PREDICATE("any_of");
    return std::apply([&args...](const auto &...preds)
                      { return (preds(args...) || ...); }, predicates_);
  }
};

/**
 * Tests if none of the provided predicates return true
 */
template <typename... P>
struct none_of
{
  std::tuple<P...> predicates_;

  template <typename... Preds>
  TUNIT_CONSTEXPR none_of(Preds &&...preds) noexcept : predicates_(std::forward<Preds>(preds)...) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args &&...args) const
  {
    TUNIT_TRACE_PREDICATE("none_of");
    return std::apply([&args...](const auto &...preds)
                      { return !(preds(args...) || ...); }, predicates_);
  }
};

/**
 * Conditional predicate that applies different predicates based on a condition
 */
template <typename C, typename T, typename E>
struct conditional
{
  C condition_;
  T then_pred_;
  E else_pred_;

  template <typename CondPred, typename ThenPred, typename ElsePred>
  TUNIT_CONSTEXPR explicit conditional(CondPred &&condition, ThenPred &&then_pred, ElsePred &&else_pred) noexcept : condition_(std::forward<CondPred>(condition)), then_pred_(std::forward<ThenPred>(then_pred)), else_pred_(std::forward<ElsePred>(else_pred)) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args &&...args) const
  {
    TUNIT_TRACE_PREDICATE("conditional");
    return condition_(args...) ? then_pred_(args...) : else_pred_(args...);
  }
};

/**
 * Tests if exactly n of the provided predicates return true
 */
template <typename... P>
struct exactly_n_of
{
  std::tuple<P...> predicates_;
  std::size_t expected_count_;

  template <typename... Preds>
  TUNIT_CONSTEXPR exactly_n_of(std::size_t n, Preds &&...preds) noexcept : predicates_(std::forward<Preds>(preds)...), expected_count_(n) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args &&...args) const
  {
    TUNIT_TRACE_PREDICATE("exactly_n_of");
    std::size_t count = 0;
    std::apply([&count, &args...](const auto &...preds)
               { ((count += preds(args...) ? 1 : 0), ...); }, predicates_);
    return count == expected_count_;
  }
};

/**
 * Tests if at least n of the provided predicates return true
 */
template <typename... P>
struct at_least_n_of
{
  std::tuple<P...> predicates_;
  std::size_t min_count_;

  template <typename... Preds>
  TUNIT_CONSTEXPR at_least_n_of(std::size_t n, Preds &&...preds) noexcept : predicates_(std::forward<Preds>(preds)...), min_count_(n) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args &&...args) const
  {
    TUNIT_TRACE_PREDICATE("at_least_n_of");
    std::size_t count = 0;
    std::apply([&count, &args...](const auto &...preds)
               { ((count += preds(args...) ? 1 : 0), ...); }, predicates_);
    return count >= min_count_;
  }
};

/**
 * Tests if at most n of the provided predicates return true
 */
template <typename... P>
struct at_most_n_of
{
  std::tuple<P...> predicates_;
  std::size_t max_count_;

  template <typename... Preds>
  TUNIT_CONSTEXPR at_most_n_of(std::size_t n, Preds &&...preds) noexcept : predicates_(std::forward<Preds>(preds)...), max_count_(n) {}

  template <typename... Args>
  TUNIT_CONSTEXPR bool operator()(Args &&...args) const
  {
    TUNIT_TRACE_PREDICATE("at_most_n_of");
    std::size_t count = 0;
    std::apply([&count, &args...](const auto &...preds)
               { ((count += preds(args...) ? 1 : 0), ...); }, predicates_);
    return count <= max_count_;
  }
};

// ********************** Deduction Guides **********************

// TODO: Implement automatic deduction guides for complex predicates

// Deduction guide for all_of
template <typename... Preds>
all_of(Preds &&...) -> all_of<std::decay_t<Preds>...>;

// Deduction guide for any_of
template <typename... Preds>
any_of(Preds &&...) -> any_of<std::decay_t<Preds>...>;

// Deduction guide for none_of
template <typename... Preds>
none_of(Preds &&...) -> none_of<std::decay_t<Preds>...>;

// Deduction guide for conditional
template <typename C, typename T, typename E>
conditional(C &&, T &&, E &&) -> conditional<std::decay_t<C>, std::decay_t<T>, std::decay_t<E>>;

// Deduction guide for exactly_n_of
template <typename... Preds>
exactly_n_of(std::size_t, Preds &&...) -> exactly_n_of<std::decay_t<Preds>...>;

// Deduction guide for at_least_n_of
template <typename... Preds>
at_least_n_of(std::size_t, Preds &&...) -> at_least_n_of<std::decay_t<Preds>...>;

// Deduction guide for at_most_n_of
template <typename... Preds>
at_most_n_of(std::size_t, Preds &&...) -> at_most_n_of<std::decay_t<Preds>...>;

} // namespace predicates
} // namespace tunit