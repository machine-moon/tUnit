#pragma once

#include <functional>

namespace tunit {
namespace predicates {

// ==================== Basic Comparison Predicates ====================

using is_equal = std::equal_to<>;
using is_not_equal = std::not_equal_to<>;
using is_greater = std::greater<>;
using is_less = std::less<>;
using is_greater_equal = std::greater_equal<>;
using is_less_equal = std::less_equal<>;

}  // namespace predicates
}  // namespace tunit
