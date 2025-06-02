#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

#include "../predicate_config.h"

namespace tunit {
namespace predicates {

// ********************** Container Element Predicates **********************

/**
 * Tests if a container contains a specific element
 */
struct contains_element {
  TUNIT_CONSTEXPR contains_element() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T& c1, const U& element) const {
    TUNIT_TRACE_PREDICATE("contains_element");
    // Argument-Dependent Lookup (ADL)
    using std::begin, std::end;
    return std::find(begin(c1), end(c1), element) != end(c1);
  }
};

/**
 * Tests if a container contains at least one element satisfying a predicate
 */
struct contains_if {
  TUNIT_CONSTEXPR contains_if() noexcept = default;

  template <typename T, typename P>
  TUNIT_CONSTEXPR bool operator()(const T& c1, P&& pred) const {
    TUNIT_TRACE_PREDICATE("contains_if");
    using std::begin, std::end;
    return std::find_if(begin(c1), end(c1), std::forward<P>(pred)) != end(c1);
  }
};

/**
 * Tests if a container contains all elements from another container
 */
struct contains_all_elements {
  TUNIT_CONSTEXPR contains_all_elements() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T& c1, const U& c2) const {
    TUNIT_TRACE_PREDICATE("contains_all_elements");
    using std::begin, std::end;
    return std::all_of(begin(c2), end(c2), [&c1](const auto& element) { return std::find(begin(c1), end(c1), element) != end(c1); });
  }
};

/**
 * Tests if a container contains any elements from another container
 */
struct contains_any_elements {
  TUNIT_CONSTEXPR contains_any_elements() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T& c1, const U& c2) const {
    TUNIT_TRACE_PREDICATE("contains_any_elements");
    using std::begin, std::end;
    return std::any_of(begin(c2), end(c2), [&c1](const auto& element) { return std::find(begin(c1), end(c1), element) != end(c1); });
  }
};

// ********************** Container Size Predicates **********************

/**
 * Tests if a container is empty
 */
struct is_empty {
  TUNIT_CONSTEXPR is_empty() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T& c1) const {
    TUNIT_TRACE_PREDICATE("is_empty");
    return c1.empty();
  }
};

/**
 * Tests if a container is not empty
 */
struct is_not_empty {
  TUNIT_CONSTEXPR is_not_empty() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T& c1) const {
    TUNIT_TRACE_PREDICATE("is_not_empty");
    return !c1.empty();
  }
};

/**
 * Tests if a container has an exact size
 */
struct has_size {
  TUNIT_CONSTEXPR has_size() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T& c1, std::size_t expected_size) const {
    TUNIT_TRACE_PREDICATE("has_size");
    return c1.size() == expected_size;
  }
};

/**
 * Tests if a container has at least a minimum size
 */
struct has_min_size {
  TUNIT_CONSTEXPR has_min_size() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T& c1, std::size_t min_size) const {
    TUNIT_TRACE_PREDICATE("has_min_size");
    return c1.size() >= min_size;
  }
};

/**
 * Tests if a container has at most a maximum size
 */
struct has_max_size {
  TUNIT_CONSTEXPR has_max_size() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T& c1, std::size_t max_size) const {
    TUNIT_TRACE_PREDICATE("has_max_size");
    return c1.size() <= max_size;
  }
};

/**
 * Tests if a container size is within a specified range
 */
struct has_size_in_range {
  TUNIT_CONSTEXPR has_size_in_range() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T& c1, std::size_t min_size, std::size_t max_size) const {
    TUNIT_TRACE_PREDICATE("has_size_in_range");
    const auto size = c1.size();
    return size >= min_size && size <= max_size;
  }
};

// ********************** Container Algorithm Predicates **********************

/**
 * Tests if all elements in a container satisfy a predicate
 */
struct all_elements_satisfy {
  TUNIT_CONSTEXPR all_elements_satisfy() noexcept = default;

  template <typename T, typename P>
  TUNIT_CONSTEXPR bool operator()(const T& c1, P&& pred) const {
    TUNIT_TRACE_PREDICATE("all_elements_satisfy");
    using std::begin, std::end;
    return std::all_of(begin(c1), end(c1), std::forward<P>(pred));
  }
};

/**
 * Tests if any element in a container satisfies a predicate
 */
struct any_element_satisfies {
  TUNIT_CONSTEXPR any_element_satisfies() noexcept = default;

  template <typename T, typename P>
  TUNIT_CONSTEXPR bool operator()(const T& c1, P&& pred) const {
    TUNIT_TRACE_PREDICATE("any_element_satisfies");
    using std::begin, std::end;
    return std::any_of(begin(c1), end(c1), std::forward<P>(pred));
  }
};

/**
 * Tests if no elements in a container satisfy a predicate
 */
struct no_elements_satisfy {
  TUNIT_CONSTEXPR no_elements_satisfy() noexcept = default;

  template <typename T, typename P>
  TUNIT_CONSTEXPR bool operator()(const T& c1, P&& pred) const {
    TUNIT_TRACE_PREDICATE("no_elements_satisfy");
    using std::begin, std::end;
    return std::none_of(begin(c1), end(c1), std::forward<P>(pred));
  }
};

/**
 * Counts the number of elements satisfying a predicate
 */
struct count_elements_satisfying {
  TUNIT_CONSTEXPR count_elements_satisfying() noexcept = default;

  template <typename T, typename P>
  TUNIT_CONSTEXPR auto operator()(const T& c1, P&& pred) const {
    TUNIT_TRACE_PREDICATE("count_elements_satisfying");
    using std::begin, std::end;
    return std::count_if(begin(c1), end(c1), std::forward<P>(pred));
  }
};

/**
 * Tests if exactly n elements satisfy a predicate
 */
struct exactly_n_elements_satisfy {
  TUNIT_CONSTEXPR exactly_n_elements_satisfy() noexcept = default;

  template <typename T, typename P>
  TUNIT_CONSTEXPR bool operator()(const T& c1, std::size_t expected_count, P&& pred) const {
    TUNIT_TRACE_PREDICATE("exactly_n_elements_satisfy");
    using std::begin, std::end;
    return static_cast<std::size_t>(std::count_if(begin(c1), end(c1), std::forward<P>(pred))) == expected_count;
  }
};

/**
 * Tests if at least n elements satisfy a predicate
 */
struct at_least_n_elements_satisfy {
  TUNIT_CONSTEXPR at_least_n_elements_satisfy() noexcept = default;

  template <typename T, typename P>
  TUNIT_CONSTEXPR bool operator()(const T& c1, std::size_t min_count, P&& pred) const {
    TUNIT_TRACE_PREDICATE("at_least_n_elements_satisfy");
    using std::begin, std::end;
    return static_cast<std::size_t>(std::count_if(begin(c1), end(c1), std::forward<P>(pred))) >= min_count;
  }
};

/**
 * Tests if at most n elements satisfy a predicate
 */
struct at_most_n_elements_satisfy {
  TUNIT_CONSTEXPR at_most_n_elements_satisfy() noexcept = default;

  template <typename T, typename P>
  TUNIT_CONSTEXPR bool operator()(const T& c1, std::size_t max_count, P&& pred) const {
    TUNIT_TRACE_PREDICATE("at_most_n_elements_satisfy");
    using std::begin, std::end;
    return static_cast<std::size_t>(std::count_if(begin(c1), end(c1), std::forward<P>(pred))) <= max_count;
  }
};

// ********************** Container Ordering Predicates **********************

/**
 * Tests if a container is sorted in ascending order
 */
struct is_sorted {
  TUNIT_CONSTEXPR is_sorted() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T& c1) const {
    TUNIT_TRACE_PREDICATE("is_sorted");
    using std::begin, std::end;
    return std::is_sorted(begin(c1), end(c1));
  }

  template <typename T, typename P>
  TUNIT_CONSTEXPR bool operator()(const T& c1, P&& comp) const {
    TUNIT_TRACE_PREDICATE("is_sorted");
    using std::begin, std::end;
    return std::is_sorted(begin(c1), end(c1), std::forward<P>(comp));
  }
};

/**
 * Tests if a container is sorted in descending order
 */
struct is_reverse_sorted {
  TUNIT_CONSTEXPR is_reverse_sorted() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T& c1) const {
    TUNIT_TRACE_PREDICATE("is_reverse_sorted");
    using std::begin, std::end;
    return std::is_sorted(begin(c1), end(c1), std::greater<>{});
  }
};

/**
 * Tests if all elements in a container are unique
 */
struct is_unique {
  TUNIT_CONSTEXPR is_unique() noexcept = default;

  template <typename T>
  TUNIT_CONSTEXPR bool operator()(const T& c1) const {
    TUNIT_TRACE_PREDICATE("is_unique");
    using std::begin, std::end;
    auto sorted_copy = c1;
    std::sort(begin(sorted_copy), end(sorted_copy));
    return std::adjacent_find(begin(sorted_copy), end(sorted_copy)) == end(sorted_copy);
  }
};

// ********************** Container Equality Predicates **********************

/**
 * Tests if two containers are equal element by element
 */
struct containers_equal {
  TUNIT_CONSTEXPR containers_equal() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T& c1, const U& c2) const {
    TUNIT_TRACE_PREDICATE("containers_equal");
    using std::begin, std::end;
    return std::equal(begin(c1), end(c1), begin(c2), end(c2));
  }

  template <typename T, typename U, typename P>
  TUNIT_CONSTEXPR bool operator()(const T& c1, const U& c2, P&& pred) const {
    TUNIT_TRACE_PREDICATE("containers_equal");
    using std::begin, std::end;
    return std::equal(begin(c1), end(c1), begin(c2), end(c2), std::forward<P>(pred));
  }
};

/**
 * Tests if one container is a permutation of another
 */
struct is_permutation_of {
  TUNIT_CONSTEXPR is_permutation_of() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T& c1, const U& c2) const {
    TUNIT_TRACE_PREDICATE("is_permutation_of");
    using std::begin, std::end;
    return std::is_permutation(begin(c1), end(c1), begin(c2), end(c2));
  }

  template <typename T, typename U, typename P>
  TUNIT_CONSTEXPR bool operator()(const T& c1, const U& c2, P&& pred) const {
    TUNIT_TRACE_PREDICATE("is_permutation_of");
    using std::begin, std::end;
    return std::is_permutation(begin(c1), end(c1), begin(c2), end(c2), std::forward<P>(pred));
  }
};

}  // namespace predicates
}  // namespace tunit