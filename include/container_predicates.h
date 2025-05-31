#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace tunit
{
namespace predicates
{

// ==================== Container Element Predicates ====================

struct contains_element
{
    template<typename Container, typename T>
    constexpr bool operator()(const Container& container, const T& element) const noexcept
    {
        using std::begin, std::end;
        return std::find(begin(container), end(container), element) != end(container);
    }
};

struct contains_if
{
    template<typename Container, typename Predicate>
    constexpr bool operator()(const Container& container, Predicate&& predicate) const noexcept
    {
        using std::begin, std::end;
        return std::find_if(begin(container), end(container), std::forward<Predicate>(predicate)) != end(container);
    }
};

struct contains_all_elements
{
    template<typename Container, typename OtherContainer>
    constexpr bool operator()(const Container& container, const OtherContainer& elements) const noexcept
    {
        using std::begin, std::end;
        return std::all_of(begin(elements), end(elements), [&container](const auto& element) {
            return std::find(begin(container), end(container), element) != end(container);
        });
    }
};

struct contains_any_elements
{
    template<typename Container, typename OtherContainer>
    constexpr bool operator()(const Container& container, const OtherContainer& elements) const noexcept
    {
        using std::begin, std::end;
        return std::any_of(begin(elements), end(elements), [&container](const auto& element) {
            return std::find(begin(container), end(container), element) != end(container);
        });
    }
};

// ==================== Container Size Predicates ====================

struct is_empty
{
    template<typename Container>
    constexpr bool operator()(const Container& container) const noexcept
    {
        return container.empty();
    }
};

struct is_not_empty
{
    template<typename Container>
    constexpr bool operator()(const Container& container) const noexcept
    {
        return !container.empty();
    }
};

struct has_size
{
    template<typename Container>
    constexpr bool operator()(const Container& container, std::size_t expected_size) const noexcept
    {
        return container.size() == expected_size;
    }
};

struct has_min_size
{
    template<typename Container>
    constexpr bool operator()(const Container& container, std::size_t min_size) const noexcept
    {
        return container.size() >= min_size;
    }
};

struct has_max_size
{
    template<typename Container>
    constexpr bool operator()(const Container& container, std::size_t max_size) const noexcept
    {
        return container.size() <= max_size;
    }
};

struct has_size_in_range
{
    template<typename Container>
    constexpr bool operator()(const Container& container, std::size_t min_size, std::size_t max_size) const noexcept
    {
        const auto size = container.size();
        return size >= min_size && size <= max_size;
    }
};

// ==================== Container Algorithm Predicates ====================

struct all_elements_satisfy
{
    template<typename Container, typename Predicate>
    constexpr bool operator()(const Container& container, Predicate&& predicate) const noexcept
    {
        using std::begin, std::end;
        return std::all_of(begin(container), end(container), std::forward<Predicate>(predicate));
    }
};

struct any_element_satisfies
{
    template<typename Container, typename Predicate>
    constexpr bool operator()(const Container& container, Predicate&& predicate) const noexcept
    {
        using std::begin, std::end;
        return std::any_of(begin(container), end(container), std::forward<Predicate>(predicate));
    }
};

struct no_elements_satisfy
{
    template<typename Container, typename Predicate>
    constexpr bool operator()(const Container& container, Predicate&& predicate) const noexcept
    {
        using std::begin, std::end;
        return std::none_of(begin(container), end(container), std::forward<Predicate>(predicate));
    }
};

struct count_elements_satisfying
{
    template<typename Container, typename Predicate>
    constexpr auto operator()(const Container& container, Predicate&& predicate) const noexcept
    {
        using std::begin, std::end;
        return std::count_if(begin(container), end(container), std::forward<Predicate>(predicate));
    }
};

struct exactly_n_elements_satisfy
{
    template<typename Container, typename Predicate>
    constexpr bool operator()(const Container& container, std::size_t expected_count, Predicate&& predicate) const noexcept
    {
        using std::begin, std::end;
        return static_cast<std::size_t>(std::count_if(begin(container), end(container), std::forward<Predicate>(predicate))) == expected_count;
    }
};

struct at_least_n_elements_satisfy
{
    template<typename Container, typename Predicate>
    constexpr bool operator()(const Container& container, std::size_t min_count, Predicate&& predicate) const noexcept
    {
        using std::begin, std::end;
        return static_cast<std::size_t>(std::count_if(begin(container), end(container), std::forward<Predicate>(predicate))) >= min_count;
    }
};

struct at_most_n_elements_satisfy
{
    template<typename Container, typename Predicate>
    constexpr bool operator()(const Container& container, std::size_t max_count, Predicate&& predicate) const noexcept
    {
        using std::begin, std::end;
        return static_cast<std::size_t>(std::count_if(begin(container), end(container), std::forward<Predicate>(predicate))) <= max_count;
    }
};

// ==================== Container Ordering Predicates ====================

struct is_sorted
{
    template<typename Container>
    constexpr bool operator()(const Container& container) const noexcept
    {
        using std::begin, std::end;
        return std::is_sorted(begin(container), end(container));
    }

    template<typename Container, typename Compare>
    constexpr bool operator()(const Container& container, Compare&& comp) const noexcept
    {
        using std::begin, std::end;
        return std::is_sorted(begin(container), end(container), std::forward<Compare>(comp));
    }
};

struct is_reverse_sorted
{
    template<typename Container>
    constexpr bool operator()(const Container& container) const noexcept
    {
        using std::begin, std::end;
        return std::is_sorted(begin(container), end(container), std::greater<>{});
    }
};

struct is_unique
{
    template<typename Container>
    constexpr bool operator()(const Container& container) const noexcept
    {
        using std::begin, std::end;
        auto sorted_copy = container;
        std::sort(begin(sorted_copy), end(sorted_copy));
        return std::adjacent_find(begin(sorted_copy), end(sorted_copy)) == end(sorted_copy);
    }
};

// ==================== Container Equality Predicates ====================

struct containers_equal
{
    template<typename Container1, typename Container2>
    constexpr bool operator()(const Container1& c1, const Container2& c2) const noexcept
    {
        using std::begin, std::end;
        return std::equal(begin(c1), end(c1), begin(c2), end(c2));
    }

    template<typename Container1, typename Container2, typename BinaryPredicate>
    constexpr bool operator()(const Container1& c1, const Container2& c2, BinaryPredicate&& pred) const noexcept
    {
        using std::begin, std::end;
        return std::equal(begin(c1), end(c1), begin(c2), end(c2), std::forward<BinaryPredicate>(pred));
    }
};

struct is_permutation_of
{
    template<typename Container1, typename Container2>
    constexpr bool operator()(const Container1& c1, const Container2& c2) const noexcept
    {
        using std::begin, std::end;
        return std::is_permutation(begin(c1), end(c1), begin(c2), end(c2));
    }

    template<typename Container1, typename Container2, typename BinaryPredicate>
    constexpr bool operator()(const Container1& c1, const Container2& c2, BinaryPredicate&& pred) const noexcept
    {
        using std::begin, std::end;
        return std::is_permutation(begin(c1), end(c1), begin(c2), end(c2), std::forward<BinaryPredicate>(pred));
    }
};

} // namespace predicates
} // namespace tunit