#pragma once

#include <string>
#include <string_view>

namespace tunit {
namespace predicates {

// ==================== String Comparison Predicates ====================

struct contains_substring {
  template <typename T, typename U>
  constexpr bool operator()(const T& str, const U& substr) const noexcept {
    std::string_view str_view{str};
    std::string_view substr_view{substr};
    return str_view.find(substr_view) != std::string_view::npos;
  }
};

struct starts_with {
  template <typename T, typename U>
  constexpr bool operator()(const T& str, const U& prefix) const noexcept {
    std::string_view str_view{str};
    std::string_view prefix_view{prefix};
    return str_view.size() >= prefix_view.size() && str_view.substr(0, prefix_view.size()) == prefix_view;
  }
};

struct ends_with {
  template <typename T, typename U>
  constexpr bool operator()(const T& str, const U& suffix) const noexcept {
    std::string_view str_view{str};
    std::string_view suffix_view{suffix};
    return str_view.size() >= suffix_view.size() && str_view.substr(str_view.size() - suffix_view.size()) == suffix_view;
  }
};

}  // namespace predicates
}  // namespace tunit
