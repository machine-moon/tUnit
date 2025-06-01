#pragma once

// Collections
#include "collections/container_predicates.h"

// Common
#include "common/comparison_predicates.h"
#include "common/numeric_predicates.h"
#include "common/range_predicates.h"
#include "common/string_predicates.h"

// Custom
#include "custom/custom_predicates.h"

// Stateful
#include "stateful/complex_predicates.h"
#include "stateful/logical_predicates.h"

// This header provides a single inclusion point for all predicate components in the tunit library.
//
// Category Overview:
// - collections/: Container and sequence operations (contains, is_empty, is_sorted, etc.)
// - common/: Fundamental stateless predicates (comparison, numeric, range, string)
// - custom/: Specialized domain predicates (palindrome, perfect_square, etc.)
// - stateful/: Complex predicates that store configuration (all_of, any_of, logical combinators)
