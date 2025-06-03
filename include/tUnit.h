/**
 * @file tUnit.h
 * @brief Single-header include for the complete tUnit testing framework.
 *
 * This header aggregates all core components of the tUnit testing framework:
 *   - Predicates:         predicates/all_predicates.h
 *   - Assertions:         tUnit/assertion.h
 *   - Test cases:         tUnit/test_case.h
 *   - Test orchestrator:  tUnit/test_orchestrator.h
 *   - Test suites:        tUnit/test_suite.h
 *   - release asserts:    utils/release_asserts.h
 *   - Trace utilities:    utils/trace_support.h
 *   - Core evaluator:     evaluator.h
 */
#pragma once

#include "predicates/all_predicates.h"

#include "tUnit/assertion.h"
#include "tUnit/test_case.h"
#include "tUnit/test_orchestrator.h"
#include "tUnit/test_suite.h"

// #include "utils/release_asserts.h"
#include "utils/trace_support.h"

#include "evaluator.h"
