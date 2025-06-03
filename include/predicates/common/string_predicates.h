#pragma once

#include <string>
#include <string_view>

#include "predicates/predicate_config.h"

namespace tUnit
{
namespace predicates
{

/**
 * Tests if a string contains a specified substring
 */
struct contains_substring
{
  TUNIT_CONSTEXPR contains_substring() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T &str, const U &substr) const
  {
    TUNIT_TRACE_PREDICATE("contains_substring");
    std::string_view str_view{str};
    std::string_view substr_view{substr};
    return str_view.find(substr_view) != std::string_view::npos;
  }
};

/**
 * Tests if a string starts with a specified prefix
 */
struct starts_with
{
  TUNIT_CONSTEXPR starts_with() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T &str, const U &prefix) const
  {
    TUNIT_TRACE_PREDICATE("starts_with");
    std::string_view str_view{str};
    std::string_view prefix_view{prefix};
    return str_view.size() >= prefix_view.size() && str_view.substr(0, prefix_view.size()) == prefix_view;
  }
};

/**
 * Tests if a string ends with a specified suffix
 */
struct ends_with
{
  TUNIT_CONSTEXPR ends_with() noexcept = default;

  template <typename T, typename U>
  TUNIT_CONSTEXPR bool operator()(const T &str, const U &suffix) const
  {
    TUNIT_TRACE_PREDICATE("ends_with");
    std::string_view str_view{str};
    std::string_view suffix_view{suffix};
    return str_view.size() >= suffix_view.size() && str_view.substr(str_view.size() - suffix_view.size()) == suffix_view;
  }
};

} // namespace predicates
} // namespace tUnit
