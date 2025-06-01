#pragma once

// Predicate Configuration for TUnit
//
// Two modes available:
// - PRODUCTION MODE (1): constexpr enabled, tracing disabled - for release builds
// - DEBUG/DEV MODE (0): constexpr disabled, tracing enabled - for development and debugging

// Set to 1 for production mode (constexpr + fast compilation)
// Set to 0 for debug/dev mode (tracing + exception handling)
#ifndef TUNIT_PRODUCTION_MODE
#define TUNIT_PRODUCTION_MODE 1
#endif

// Internal configuration logic
#if TUNIT_PRODUCTION_MODE
// Production mode: constexpr enabled, tracing disabled
#define TUNIT_CONSTEXPR constexpr
#define TUNIT_TRACE_PREDICATE(name) ((void)0)
#else
// Debug/Dev mode: constexpr disabled, tracing enabled
#define TUNIT_CONSTEXPR
#include "../utils/trace_support.h"
#define TUNIT_TRACE_PREDICATE(name) TUNIT_TRACE_SCOPE("predicate: " name)
#endif
