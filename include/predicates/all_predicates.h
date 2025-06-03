#pragma once
// This header provides a single inclusion point for all predicate components in the tunit library.

#include "collections/container_predicates.h"

#include "common/comparison_predicates.h"
#include "common/numeric_predicates.h"
#include "common/range_predicates.h"
#include "common/string_predicates.h"

#include "custom/custom_predicates.h"

#include "stateful/complex_predicates.h"
#include "stateful/logical_predicates.h"

/**
 * - collections/: Predicates for container and sequence operations (e.g., contains, is_empty, is_sorted).
 * - common/: Fundamental stateless predicates, including comparison, numeric, range, and string operations.
 * - custom/: Specialized domain-specific predicates (e.g., palindrome, perfect_square).
 * - stateful/: Complex predicates that maintain internal state or configuration (e.g., all_of, any_of, logical combinators).
 *
 * Including this header will provide access to all predicate functionalities offered by tunit.
 */