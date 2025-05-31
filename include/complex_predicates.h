#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace tunit
{
namespace predicates
{

// ==================== Complex Predicate Compositions ====================

template<typename... Predicates>
struct all_of
{
    std::tuple<Predicates...> predicates_;

    template<typename... Preds>
    constexpr explicit all_of(Preds&&... preds) noexcept
        : predicates_(std::forward<Preds>(preds)...)
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        return std::apply([&args...](const auto&... preds) { return (preds(args...) && ...); },
                          predicates_);
    }
};

template<typename... Predicates>
struct any_of
{
    std::tuple<Predicates...> predicates_;

    template<typename... Preds>
    constexpr explicit any_of(Preds&&... preds) noexcept
        : predicates_(std::forward<Preds>(preds)...)
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        return std::apply([&args...](const auto&... preds) { return (preds(args...) || ...); },
                          predicates_);
    }
};

template<typename... Predicates>
struct none_of
{
    std::tuple<Predicates...> predicates_;

    template<typename... Preds>
    constexpr explicit none_of(Preds&&... preds) noexcept
        : predicates_(std::forward<Preds>(preds)...)
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        return std::apply([&args...](const auto&... preds) { return !(preds(args...) || ...); },
                          predicates_);
    }
};

template<typename ConditionPred, typename ThenPred, typename ElsePred>
struct conditional
{
    ConditionPred condition_;
    ThenPred then_pred_;
    ElsePred else_pred_;

    template<typename C, typename T, typename E>
    constexpr conditional(C&& condition, T&& then_pred, E&& else_pred) noexcept
        : condition_(std::forward<C>(condition))
        , then_pred_(std::forward<T>(then_pred))
        , else_pred_(std::forward<E>(else_pred))
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        return condition_(args...) ? then_pred_(args...) : else_pred_(args...);
    }
};

template<typename... Predicates>
struct exactly_n_of
{
    std::tuple<Predicates...> predicates_;
    std::size_t expected_count_;

    template<typename... Preds>
    constexpr exactly_n_of(std::size_t n, Preds&&... preds) noexcept
        : predicates_(std::forward<Preds>(preds)...)
        , expected_count_(n)
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        std::size_t count = 0;
        std::apply(
            [&count, &args...](const auto&... preds) { ((count += preds(args...) ? 1 : 0), ...); },
            predicates_);
        return count == expected_count_;
    }
};

template<typename... Predicates>
struct at_least_n_of
{
    std::tuple<Predicates...> predicates_;
    std::size_t min_count_;

    template<typename... Preds>
    constexpr at_least_n_of(std::size_t n, Preds&&... preds) noexcept
        : predicates_(std::forward<Preds>(preds)...)
        , min_count_(n)
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        std::size_t count = 0;
        std::apply(
            [&count, &args...](const auto&... preds) { ((count += preds(args...) ? 1 : 0), ...); },
            predicates_);
        return count >= min_count_;
    }
};

template<typename... Predicates>
struct at_most_n_of
{
    std::tuple<Predicates...> predicates_;
    std::size_t max_count_;

    template<typename... Preds>
    constexpr at_most_n_of(std::size_t n, Preds&&... preds) noexcept
        : predicates_(std::forward<Preds>(preds)...)
        , max_count_(n)
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        std::size_t count = 0;
        std::apply(
            [&count, &args...](const auto&... preds) { ((count += preds(args...) ? 1 : 0), ...); },
            predicates_);
        return count <= max_count_;
    }
};

} // namespace predicates
} // namespace tunit