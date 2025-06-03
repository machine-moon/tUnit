/**
 * Release-mode assertion macros that work even in release builds
 */
#pragma once

// from: github.com/attwoodn/cpp-expression-tree/blob/main/tests/release_asserts.hpp

#ifdef NDEBUG
#undef NDEBUG
#include <cassert>
#define NDEBUG
#else
#include <cassert>
#endif