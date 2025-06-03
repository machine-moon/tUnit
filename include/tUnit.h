#pragma once

// Single header

#include "predicates/all_predicates.h"

#include "tUnit/assertion.h"
#include "tUnit/test_case.h"
#include "tUnit/test_orchestrator.h"
#include "tUnit/test_suite.h"

// #include "utils/release_asserts.h" //conflicts with ASSERT macro
#include "utils/trace_support.h"

#include "evaluator.h"

// This header provides all core components of the tunit library:
// - All predicates from predicates/all_predicates.h
// - tUnit testing framework from tUnit/test_case.h, tUnit/test_orchestrator.h, tUnit/test_suite.h, and tUnit/assertion.h
// - Release-mode assertions from utils/release_asserts.h
// - Trace support utilities from utils/trace_support.h
// - Core evaluator functionality from evaluator.h
