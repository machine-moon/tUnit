#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace tunit {
namespace predicates {

// ==================== Complex Predicate Compositions ====================

template <typename... P>
struct all_of {
  std::tuple<P...> predicates_;

  template <typename... Preds>
  constexpr explicit all_of(Preds&&... preds) noexcept : predicates_(std::forward<Preds>(preds)...) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return std::apply([&args...](const auto&... preds) { return (preds(args...) && ...); }, predicates_);
  }
};

template <typename... P>
struct any_of {
  std::tuple<P...> predicates_;

  template <typename... Preds>
  constexpr explicit any_of(Preds&&... preds) noexcept : predicates_(std::forward<Preds>(preds)...) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return std::apply([&args...](const auto&... preds) { return (preds(args...) || ...); }, predicates_);
  }
};

template <typename... P>
struct none_of {
  std::tuple<P...> predicates_;

  template <typename... Preds>
  constexpr explicit none_of(Preds&&... preds) noexcept : predicates_(std::forward<Preds>(preds)...) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return std::apply([&args...](const auto&... preds) { return !(preds(args...) || ...); }, predicates_);
  }
};

template <typename C, typename T, typename E>
struct conditional {
  C condition_;
  T then_pred_;
  E else_pred_;

  template <typename CondPred, typename ThenPred, typename ElsePred>
  constexpr conditional(CondPred&& condition, ThenPred&& then_pred, ElsePred&& else_pred) noexcept : condition_(std::forward<CondPred>(condition)), then_pred_(std::forward<ThenPred>(then_pred)), else_pred_(std::forward<ElsePred>(else_pred)) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    return condition_(args...) ? then_pred_(args...) : else_pred_(args...);
  }
};

template <typename... P>
struct exactly_n_of {
  std::tuple<P...> predicates_;
  std::size_t expected_count_;

  template <typename... Preds>
  constexpr exactly_n_of(std::size_t n, Preds&&... preds) noexcept : predicates_(std::forward<Preds>(preds)...), expected_count_(n) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    std::size_t count = 0;
    std::apply([&count, &args...](const auto&... preds) { ((count += preds(args...) ? 1 : 0), ...); }, predicates_);
    return count == expected_count_;
  }
};

template <typename... P>
struct at_least_n_of {
  std::tuple<P...> predicates_;
  std::size_t min_count_;

  template <typename... Preds>
  constexpr at_least_n_of(std::size_t n, Preds&&... preds) noexcept : predicates_(std::forward<Preds>(preds)...), min_count_(n) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    std::size_t count = 0;
    std::apply([&count, &args...](const auto&... preds) { ((count += preds(args...) ? 1 : 0), ...); }, predicates_);
    return count >= min_count_;
  }
};

template <typename... P>
struct at_most_n_of {
  std::tuple<P...> predicates_;
  std::size_t max_count_;

  template <typename... Preds>
  constexpr at_most_n_of(std::size_t n, Preds&&... preds) noexcept : predicates_(std::forward<Preds>(preds)...), max_count_(n) {}

  template <typename... Args>
  constexpr bool operator()(Args&&... args) const noexcept {
    std::size_t count = 0;
    std::apply([&count, &args...](const auto&... preds) { ((count += preds(args...) ? 1 : 0), ...); }, predicates_);
    return count <= max_count_;
  }
};

// ==================== Deduction Guides ====================

// Deduction guide for all_of
template <typename... Preds>
all_of(Preds&&...) -> all_of<std::decay_t<Preds>...>;

// Deduction guide for any_of
template <typename... Preds>
any_of(Preds&&...) -> any_of<std::decay_t<Preds>...>;

// Deduction guide for none_of
template <typename... Preds>
none_of(Preds&&...) -> none_of<std::decay_t<Preds>...>;

// Deduction guide for conditional
template <typename C, typename T, typename E>
conditional(C&&, T&&, E&&) -> conditional<std::decay_t<C>, std::decay_t<T>, std::decay_t<E>>;

// Deduction guide for exactly_n_of
template <typename... Preds>
exactly_n_of(std::size_t, Preds&&...) -> exactly_n_of<std::decay_t<Preds>...>;

// Deduction guide for at_least_n_of
template <typename... Preds>
at_least_n_of(std::size_t, Preds&&...) -> at_least_n_of<std::decay_t<Preds>...>;

// Deduction guide for at_most_n_of
template <typename... Preds>
at_most_n_of(std::size_t, Preds&&...) -> at_most_n_of<std::decay_t<Preds>...>;

}  // namespace predicates
}  // namespace tunit