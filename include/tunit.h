#pragma once

// Single header
#include "evaluator.h"

// todo: rename this:
#include "orchestrator/assertion.h"
#include "orchestrator/orchestrator.h"
#include "orchestrator/suite.h"
#include "orchestrator/test.h"

#include "predicates/all_predicates.h"
// #include "utils/release_asserts.h" //conflicts with ASSERT macro
#include "utils/trace_support.h"

// This header provides all core components of the tunit library:
// - Core evaluator functionality from evaluator.h
// - All predicates from predicates/all_predicates.h
// - Test orchestrator framework from orchestrator/orchestrator.h
// - Trace support utilities from utils/trace_support.h
// - Release-mode assertions from utils/release_asserts.h
