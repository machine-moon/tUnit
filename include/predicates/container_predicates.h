#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace tunit {
namespace predicates {

// ==================== Container Element Predicates ====================

struct contains_element {
  template <typename T, typename U>
  constexpr bool operator()(const T& c1, const U& element) const noexcept {
    using std::begin, std::end;
    return std::find(begin(c1), end(c1), element) != end(c1);
  }
};

struct contains_if {
  template <typename T, typename P>
  constexpr bool operator()(const T& c1, P&& pred) const noexcept {
    using std::begin, std::end;
    return std::find_if(begin(c1), end(c1), std::forward<P>(pred)) != end(c1);
  }
};

struct contains_all_elements {
  template <typename T, typename U>
  constexpr bool operator()(const T& c1, const U& c2) const noexcept {
    using std::begin, std::end;
    return std::all_of(begin(c2), end(c2), [&c1](const auto& element) { return std::find(begin(c1), end(c1), element) != end(c1); });
  }
};

struct contains_any_elements {
  template <typename T, typename U>
  constexpr bool operator()(const T& c1, const U& c2) const noexcept {
    using std::begin, std::end;
    return std::any_of(begin(c2), end(c2), [&c1](const auto& element) { return std::find(begin(c1), end(c1), element) != end(c1); });
  }
};

// ==================== Container Size Predicates ====================

struct is_empty {
  template <typename T>
  constexpr bool operator()(const T& c1) const noexcept {
    return c1.empty();
  }
};

struct is_not_empty {
  template <typename T>
  constexpr bool operator()(const T& c1) const noexcept {
    return !c1.empty();
  }
};

struct has_size {
  template <typename T>
  constexpr bool operator()(const T& c1, std::size_t expected_size) const noexcept {
    return c1.size() == expected_size;
  }
};

struct has_min_size {
  template <typename T>
  constexpr bool operator()(const T& c1, std::size_t min_size) const noexcept {
    return c1.size() >= min_size;
  }
};

struct has_max_size {
  template <typename T>
  constexpr bool operator()(const T& c1, std::size_t max_size) const noexcept {
    return c1.size() <= max_size;
  }
};

struct has_size_in_range {
  template <typename T>
  constexpr bool operator()(const T& c1, std::size_t min_size, std::size_t max_size) const noexcept {
    const auto size = c1.size();
    return size >= min_size && size <= max_size;
  }
};

// ==================== Container Algorithm Predicates ====================

struct all_elements_satisfy {
  template <typename T, typename P>
  constexpr bool operator()(const T& c1, P&& pred) const noexcept {
    using std::begin, std::end;
    return std::all_of(begin(c1), end(c1), std::forward<P>(pred));
  }
};

struct any_element_satisfies {
  template <typename T, typename P>
  constexpr bool operator()(const T& c1, P&& pred) const noexcept {
    using std::begin, std::end;
    return std::any_of(begin(c1), end(c1), std::forward<P>(pred));
  }
};

struct no_elements_satisfy {
  template <typename T, typename P>
  constexpr bool operator()(const T& c1, P&& pred) const noexcept {
    using std::begin, std::end;
    return std::none_of(begin(c1), end(c1), std::forward<P>(pred));
  }
};

struct count_elements_satisfying {
  template <typename T, typename P>
  constexpr auto operator()(const T& c1, P&& pred) const noexcept {
    using std::begin, std::end;
    return std::count_if(begin(c1), end(c1), std::forward<P>(pred));
  }
};

struct exactly_n_elements_satisfy {
  template <typename T, typename P>
  constexpr bool operator()(const T& c1, std::size_t expected_count, P&& pred) const noexcept {
    using std::begin, std::end;
    return static_cast<std::size_t>(std::count_if(begin(c1), end(c1), std::forward<P>(pred))) == expected_count;
  }
};

struct at_least_n_elements_satisfy {
  template <typename T, typename P>
  constexpr bool operator()(const T& c1, std::size_t min_count, P&& pred) const noexcept {
    using std::begin, std::end;
    return static_cast<std::size_t>(std::count_if(begin(c1), end(c1), std::forward<P>(pred))) >= min_count;
  }
};

struct at_most_n_elements_satisfy {
  template <typename T, typename P>
  constexpr bool operator()(const T& c1, std::size_t max_count, P&& pred) const noexcept {
    using std::begin, std::end;
    return static_cast<std::size_t>(std::count_if(begin(c1), end(c1), std::forward<P>(pred))) <= max_count;
  }
};

// ==================== Container Ordering Predicates ====================

struct is_sorted {
  template <typename T>
  constexpr bool operator()(const T& c1) const noexcept {
    using std::begin, std::end;
    return std::is_sorted(begin(c1), end(c1));
  }

  template <typename T, typename P>
  constexpr bool operator()(const T& c1, P&& comp) const noexcept {
    using std::begin, std::end;
    return std::is_sorted(begin(c1), end(c1), std::forward<P>(comp));
  }
};

struct is_reverse_sorted {
  template <typename T>
  constexpr bool operator()(const T& c1) const noexcept {
    using std::begin, std::end;
    return std::is_sorted(begin(c1), end(c1), std::greater<>{});
  }
};

struct is_unique {
  template <typename T>
  constexpr bool operator()(const T& c1) const noexcept {
    using std::begin, std::end;
    auto sorted_copy = c1;
    std::sort(begin(sorted_copy), end(sorted_copy));
    return std::adjacent_find(begin(sorted_copy), end(sorted_copy)) == end(sorted_copy);
  }
};

// ==================== Container Equality Predicates ====================

struct containers_equal {
  template <typename T, typename U>
  constexpr bool operator()(const T& c1, const U& c2) const noexcept {
    using std::begin, std::end;
    return std::equal(begin(c1), end(c1), begin(c2), end(c2));
  }

  template <typename T, typename U, typename P>
  constexpr bool operator()(const T& c1, const U& c2, P&& pred) const noexcept {
    using std::begin, std::end;
    return std::equal(begin(c1), end(c1), begin(c2), end(c2), std::forward<P>(pred));
  }
};

struct is_permutation_of {
  template <typename T, typename U>
  constexpr bool operator()(const T& c1, const U& c2) const noexcept {
    using std::begin, std::end;
    return std::is_permutation(begin(c1), end(c1), begin(c2), end(c2));
  }

  template <typename T, typename U, typename P>
  constexpr bool operator()(const T& c1, const U& c2, P&& pred) const noexcept {
    using std::begin, std::end;
    return std::is_permutation(begin(c1), end(c1), begin(c2), end(c2), std::forward<P>(pred));
  }
};

}  // namespace predicates
}  // namespace tunit