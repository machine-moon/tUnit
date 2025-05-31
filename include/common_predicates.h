#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace tunit
{
namespace predicates
{

// ==================== Basic Predicates ====================

using is_equal = std::equal_to<>;
using is_not_equal = std::not_equal_to<>;
using is_greater = std::greater<>;
using is_less = std::less<>;
using is_greater_equal = std::greater_equal<>;
using is_less_equal = std::less_equal<>;

// ==================== String Comparisons ====================

struct contains_substring
{
    template<typename T, typename U>
    constexpr bool operator()(const T& str, const U& substr) const noexcept
    {
        std::string_view sv_str{str};
        std::string_view sv_substr{substr};
        return sv_str.find(sv_substr) != std::string_view::npos;
    }
};

struct starts_with
{
    template<typename T, typename U>
    constexpr bool operator()(const T& str, const U& prefix) const noexcept
    {
        std::string_view sv_str{str};
        std::string_view sv_prefix{prefix};
        return sv_str.size() >= sv_prefix.size() && sv_str.substr(0, sv_prefix.size()) == sv_prefix;
    }
};

struct ends_with
{
    template<typename T, typename U>
    constexpr bool operator()(const T& str, const U& suffix) const noexcept
    {
        std::string_view sv_str{str};
        std::string_view sv_suffix{suffix};
        return sv_str.size() >= sv_suffix.size() &&
               sv_str.substr(sv_str.size() - sv_suffix.size()) == sv_suffix;
    }
};

// ==================== Range ====================

struct is_in_range
{
    template<typename T>
    constexpr bool operator()(const T& value, const T& min_val, const T& max_val) const noexcept
    {
        return !is_less{}(value, min_val) && !is_less{}(max_val, value);
    }
};

struct is_out_of_range
{
    template<typename T>
    constexpr bool operator()(const T& value, const T& min_val, const T& max_val) const noexcept
    {
        return std::not_fn(is_in_range{})(value, min_val, max_val);
    }
};

// ==================== Numeric ====================

template<typename T>
constexpr bool is_integral_type_v = std::is_integral_v<std::decay_t<T>>;

struct is_even
{
    template<typename T>
    constexpr bool operator()(const T& value) const noexcept
    {
        static_assert(is_integral_type_v<T>, "is_even requires an integral type");
        return value % 2 == 0;
    }

    template<typename T, typename U>
    constexpr bool operator()(const T& lhs, const U& rhs) const noexcept
    {
        static_assert(is_integral_type_v<T>, "is_even requires an integral type");
        static_assert(is_integral_type_v<U>, "is_even requires an integral type");
        return (*this)(lhs) && (*this)(rhs);
    }
};

struct is_odd
{
    template<typename T>
    constexpr bool operator()(const T& value) const noexcept
    {
        static_assert(is_integral_type_v<T>, "is_odd requires an integral type");
        return std::not_fn(is_even{})(value);
    }

    template<typename T, typename U>
    constexpr bool operator()(const T& lhs, const U& rhs) const noexcept
    {
        static_assert(is_integral_type_v<T>, "is_odd requires an integral type");
        static_assert(is_integral_type_v<U>, "is_odd requires an integral type");
        return (*this)(lhs) && (*this)(rhs);
    }
};

struct is_positive
{
    template<typename T>
    constexpr bool operator()(const T& value) const noexcept
    {
        return std::greater<T>{}(value, T{});
    }

    template<typename T, typename U>
    constexpr bool operator()(const T& lhs, const U& rhs) const noexcept
    {
        return (*this)(lhs) && (*this)(rhs);
    }
};

struct is_negative
{
    template<typename T>
    constexpr bool operator()(const T& value) const noexcept
    {
        return std::less<T>{}(value, T{});
    }

    template<typename T, typename U>
    constexpr bool operator()(const T& lhs, const U& rhs) const noexcept
    {
        return (*this)(lhs) && (*this)(rhs);
    }
};

struct is_zero
{
    template<typename T>
    constexpr bool operator()(const T& value) const noexcept
    {
        return std::equal_to<T>{}(value, T{});
    }

    template<typename T, typename U>
    constexpr bool operator()(const T& lhs, const U& rhs) const noexcept
    {
        return std::equal_to<T>{}(lhs, T{}) && std::equal_to<U>{}(rhs, U{});
    }
};

} // namespace predicates
} // namespace tunit