#pragma once

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

namespace tunit
{
namespace predicates
{

// ==================== Logical Combinators ====================

template<typename LeftPred, typename RightPred>
struct and_
{
    LeftPred left_;
    RightPred right_;

    template<typename L, typename R>
    constexpr and_(L&& left, R&& right) noexcept
        : left_(std::forward<L>(left))
        , right_(std::forward<R>(right))
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        return left_(args...) && right_(args...);
    }
};

template<typename LeftPred, typename RightPred>
struct or_
{
    LeftPred left_;
    RightPred right_;

    template<typename L, typename R>
    constexpr or_(L&& left, R&& right) noexcept
        : left_(std::forward<L>(left))
        , right_(std::forward<R>(right))
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        return left_(args...) || right_(args...);
    }
};

template<typename LeftPred, typename RightPred>
struct xor_
{
    LeftPred left_;
    RightPred right_;

    template<typename L, typename R>
    constexpr xor_(L&& left, R&& right) noexcept
        : left_(std::forward<L>(left))
        , right_(std::forward<R>(right))
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        const bool left_result = left_(args...);
        const bool right_result = right_(args...);
        return left_result != right_result;
    }
};

template<typename Predicate>
struct not_
{
    Predicate predicate_;

    template<typename P>
    constexpr explicit not_(P&& predicate) noexcept
        : predicate_(std::forward<P>(predicate))
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        return !predicate_(args...);
    }
};

template<typename LeftPred, typename RightPred>
struct nand_
{
    LeftPred left_;
    RightPred right_;

    template<typename L, typename R>
    constexpr nand_(L&& left, R&& right) noexcept
        : left_(std::forward<L>(left))
        , right_(std::forward<R>(right))
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        return !(left_(args...) && right_(args...));
    }
};

template<typename LeftPred, typename RightPred>
struct nor_
{
    LeftPred left_;
    RightPred right_;

    template<typename L, typename R>
    constexpr nor_(L&& left, R&& right) noexcept
        : left_(std::forward<L>(left))
        , right_(std::forward<R>(right))
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        return !(left_(args...) || right_(args...));
    }
};

// (A implies B = !A || B)
template<typename AntecedentPred, typename ConsequentPred>
struct implies
{
    AntecedentPred antecedent_;
    ConsequentPred consequent_;

    template<typename A, typename C>
    constexpr implies(A&& antecedent, C&& consequent) noexcept
        : antecedent_(std::forward<A>(antecedent))
        , consequent_(std::forward<C>(consequent))
    {
    }

    template<typename... Args>
    constexpr bool operator()(Args&&... args) const noexcept
    {
        return !antecedent_(args...) || consequent_(args...);
    }
};

} // namespace predicates
} // namespace tunit
