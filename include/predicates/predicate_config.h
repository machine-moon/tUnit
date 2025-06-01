#pragma once

// Predicate Configuration for TUnit
//
// Two modes available:
// - (1): constexpr enabled, tracing disabled - for release builds (saves CI/CD minutes by testing at compile time)
// - (0): constexpr disabled, tracing enabled - for development and debugging

#ifndef TUNIT_MODE
#define TUNIT_MODE 1
#endif

#if TUNIT_MODE
#define TUNIT_CONSTEXPR constexpr
#define TUNIT_TRACE_PREDICATE(name) ((void)0)
#else
#define TUNIT_CONSTEXPR
#include "../utils/trace_support.h"
#define TUNIT_TRACE_PREDICATE(name) TUNIT_TRACE_SCOPE("predicate: " name)
#endif
