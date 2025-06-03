#pragma once

// Single header
#include "evaluator.h"

#include "tUnit/assertion.h"
#include "tUnit/test_case.h"
#include "tUnit/test_orchestrator.h"
#include "tUnit/test_suite.h"

#include "predicates/all_predicates.h"
#include "utils/trace_support.h"

// #include "utils/release_asserts.h" //conflicts with ASSERT macro

// This header provides all core components of the tunit library:
// - Core evaluator functionality from evaluator.h
// - Orchestrator framework from tUnit/*.h
// - All predicates from predicates/all_predicates.h
// - Trace support utilities from utils/trace_support.h
// - Release-mode assertions from utils/release_asserts.h
