#pragma once

// A) Uncomment the following line to disable constexpr for faster compilation
// B) Leave it commented to enable constexpr and allow predicate evaluation at compile-time

#define TUNIT_DISABLE_CONSTEXPR

#ifdef TUNIT_DISABLE_CONSTEXPR
    #define TUNIT_CONSTEXPR
#else
    #define TUNIT_CONSTEXPR constexpr
#endif

// Single header
#include "evaluator.h"
#include "predicates/all_predicates.h"
#include "utils/test_runner.h"
#include "utils/test_runner_utils.h"

// This header provides all core components of the tunit library:
// - Core evaluator functionality from evaluator.h
// - All predicates from predicates/all_predicates.h
// - Test runner framework (test discovery, execution, reporting) from test_runner.h and test_runner_utils.h

