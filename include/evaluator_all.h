#pragma once

// Single header
#include "common_predicates.h"
#include "complex_predicates.h"
#include "container_predicates.h"
#include "evaluator.h"
#include "logical_predicates.h"
#include "test_runner.h"

// This header provides all core components of the evaluator library:
// - Core evaluator functionality (Evaluator class) from evaluator.h
// - Basic predicates (numeric, string, comparison, range, parity, sign, zero) from
// common_predicates.h
// - Complex predicate combinators (all_of, any_of, none_of, exactly_n_of, at_least_n_of,
// at_most_n_of, conditional) from complex_predicates.h
// - Standard container predicates (element presence, size, algorithms, ordering, equality,
// permutation) from container_predicates.h
// - Logical combinators (and_, or_, xor_, not_, nand_, nor_, implies) from logical_predicates.h
