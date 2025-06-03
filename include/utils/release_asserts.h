#pragma once

/**
 * This header provides a macro for release assertions in the tunit library.
 *
 * from:
 * https://github.com/attwoodn/cpp-expression-tree/blob/main/tests/release_asserts.hpp
 */

#ifdef NDEBUG
#undef NDEBUG
#include <cassert>
#define NDEBUG
#else
#include <cassert>
#endif