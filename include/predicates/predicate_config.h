/**
 * Configuration macros for predicate compilation and tracing modes
 */
#pragma once

// Two modes available:
// - (1): constexpr enabled, predicate tracing disabled - for release builds (saves CI/CD minutes by testing at compile time)
// - (0): constexpr disabled, predicate tracing enabled - for development and debugging

#ifndef TUNIT_MODE
#define TUNIT_MODE 1 
#endif

#if 0   // TUNIT_MODE
#define TUNIT_CONSTEXPR constexpr
#define TUNIT_TRACE_PREDICATE(name) ((void)0)
#else
#define TUNIT_CONSTEXPR
#include "utils/trace_support.h"
#define TUNIT_TRACE_PREDICATE(name) TUNIT_SCOPED_TRACE("predicate: " name)
#endif
