#pragma once
// This header provides a single inclusion point for all predicate components in the tunit library.

#include "predicates/collections/container_predicates.h"

#include "predicates/common/comparison_predicates.h"
#include "predicates/common/numeric_predicates.h"
#include "predicates/common/range_predicates.h"
#include "predicates/common/string_predicates.h"

#include "predicates/custom/custom_predicates.h"

#include "predicates/stateful/complex_predicates.h"
#include "predicates/stateful/logical_predicates.h"

/**
 * - collections/: Predicates for container and sequence operations (e.g., contains, is_empty, is_sorted).
 * - common/: Fundamental stateless predicates, including comparison, numeric, range, and string operations.
 * - custom/: Specialized domain-specific predicates (e.g., palindrome, perfect_square).
 * - stateful/: Complex predicates that maintain internal state or configuration (e.g., all_of, any_of, logical combinators).
 *
 * Including this header will provide access to all predicate functionalities offered by tunit.
 */