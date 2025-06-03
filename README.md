# TUnit - Modern C++ Testing Framework

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-3.10%2B-blue.svg)](https://cmake.org/)

TUnit is a lightweight, expressive C++ testing framework designed for clarity and powerful predicate-based testing. It combines modern C++ features with comprehensive testing capabilities including trace support and automatic test discovery.

## Architecture Overview

TUnit consists of three core components working together:

### 1. Evaluator System
The central evaluation engine that processes predicates against test values:
```cpp
tUnit::Evaluator eval(10, 20, pred::is_less{});
bool result = eval.evaluate();
```

### 2. Predicate Library
A comprehensive collection of type-safe, composable predicates organized into categories:
- **Common**: Comparisons, strings, numeric operations, ranges
- **Containers**: Element operations, size checks, sorting, uniqueness
- **Logical**: AND, OR, NOT, XOR combinations
- **Complex**: Multi-predicate compositions (all_of, any_of, conditional)

### 3. Test Runner
Organizes tests into suites with automatic result tracking and reporting:
```cpp
auto& suite = tUnit::Runner::get_suite("Test Suite Name");
suite.test("description", value1, predicate, value2);
```

## Key Features

### Natural Language Testing
Tests read like specifications using descriptive predicates:
```cpp
suite.test("10 is greater than 5", 10, pred::is_greater{}, 5);
suite.test("vector contains element", numbers, pred::contains_element{}, 3);
suite.test("string is palindrome", pred::is_palindrome{}("racecar"));
```

### Predicate Composition
Complex logic through predicate combinations:
```cpp
auto complex_check = pred::all_of{pred::is_positive{}, pred::is_even{}};
auto conditional_logic = pred::conditional{condition, then_pred, else_pred};
```

### Exception Tracing
Debug mode provides detailed call stack traces:
```cpp
TUNIT_SCOPED_TRACE("Testing complex function");
// Automatic file:line information in debug output
```

## Quick Start

```cpp
#include "include/tsuite.h"
namespace pred = tUnit::predicates;

int main() {
    auto& suite = tUnit::Runner::get_suite("Basic Tests");
    
    suite.test("equality check", 5, pred::is_equal{}, 5);
    suite.test("range validation", pred::is_in_range{}(7, 1, 10));
    
    return tUnit::Runner::all_tests_passed() ? 0 : 1;
}
```

## Predicate Categories

| Category | Examples | Purpose |
|----------|----------|---------|
| **Common** | `is_equal`, `is_greater`, `contains_substring` | Basic comparisons and operations |
| **Containers** | `is_empty`, `has_size`, `is_sorted` | Container state and element operations |
| **Logical** | `and_`, `or_`, `not_`, `xor_` | Combining multiple predicates |
| **Complex** | `all_of`, `any_of`, `conditional` | Advanced multi-predicate logic |

## Build Integration

### CMake
```cmake
add_subdirectory(tunit)
target_link_libraries(your_target tunit)
```

### Make Commands
```bash
make build    # Build library and tests
make test     # Run all tests
make clean    # Clean build artifacts
```

## Technical Requirements

- C++17 or later
- CMake 3.10+
- GCC 7+, Clang 6+, or MSVC 2017+

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
