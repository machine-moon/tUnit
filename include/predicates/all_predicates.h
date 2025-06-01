#pragma once

#include "basic_predicates.h"
#include "complex_predicates.h"
#include "container_predicates.h"
#include "custom_predicates.h"
#include "logical_predicates.h"
#include "numeric_predicates.h"
#include "range_predicates.h"
#include "string_predicates.h"

// This header provides a single inclusion point for all predicate components in the tunit library.
// It includes the following headers:
//
// - basic_predicates.h: Basic comparison predicates (equal, not_equal, greater, less, etc.)
// - common_predicates.h: Provides basic predicate types such as Equal, NotEqual, Greater, Less, etc.
// - complex_predicates.h: Contains advanced predicates for compound or nested logical checks, such as AllOf, AnyOf, NoneOf.
// - container_predicates.h: Defines predicates for container types, e.g., Contains, IsEmpty, SizeIs.
// - custom_predicates.h: Allows users to define and register their own custom predicates.
// - logical_predicates.h: Supplies logical combinators and operators for predicates, such as And, Or, Not.
// - numeric_predicates.h: Numeric predicates (is_even, is_odd, is_positive, is_negative, is_zero)
// - range_predicates.h: Range validation predicates (is_in_range, is_out_of_range)
// - string_predicates.h: String-specific predicates (contains_substring, starts_with, ends_with)
