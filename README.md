# tUnit - Modern C++ Testing Framework

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)

tUnit is a lightweight, expressive C++ testing framework designed for clarity and powerful predicate-based testing. It combines modern C++ features with comprehensive testing capabilities including trace support and automatic test discovery.

## Features
### Core Testing Capabilities
- **Single Header**: Use tUnit as a single header for easy integration—just include `tUnit.h` in your project
- **Natural Language Testing**: Write tests that read like specifications with descriptive predicates
- **Type-Safe Predicates**: Comprehensive library of composable, type-safe test predicates
- **Flexible Test Organization**: Automatic test suite discovery and hierarchical organization
- **Multiple Assertion Styles**: Both `assert` and `expect` methods for different testing needs
- **Mixed Type Support**: Seamless testing across different data types (int, double, string, containers)

### Predicate Library
- **Comparison Predicates**: `is_equal`, `is_greater`, `is_less`, `is_not_equal`, `is_greater_equal`, `is_less_equal`
- **Numeric Predicates**: `is_even`, `is_odd`, `is_positive`, `is_negative`, `is_zero`
- **String Predicates**: `contains_substring`, `starts_with`, `ends_with`
- **Range Predicates**: `is_in_range`, `is_out_of_range`
- **Container Predicates**: `is_empty`, `is_not_empty`, `has_size`, `contains_element`, `is_sorted`, `is_unique`
- **Container Element Operations**: `contains_all_elements`, `contains_any_elements`, `all_elements_satisfy`, `any_element_satisfies`
- **Custom Predicates**: `is_palindrome`, `is_prime`, `is_perfect_square`

### Logical Composition
- **Basic Logic**: `and_`, `or_`, `not_`, `xor_`
- **Advanced Logic**: `nand_`, `nor_`, `implies`
- **Complex Combinations**: `all_of`, `any_of`, `none_of`
- **Conditional Logic**: `conditional` predicates with if-then-else semantics
- **Quantified Predicates**: `exactly_n_of`, `at_least_n_of`, `at_most_n_of`

### Testing Infrastructure
- **Test Orchestration**: Centralized test management with automatic suite discovery
- **Result Tracking**: Comprehensive assertion tracking and failure reporting
- **XML Output**: JUnit-compatible XML test reports for CI/CD integration
- **Command Line Interface**: Support for test filtering and output formatting
- **Summary Reports**: Detailed pass/fail statistics with failure details

### Advanced Features
- **Exception Tracing**: Detailed stack traces with scoped trace support using `TUNIT_TRACE_FUNCTION()` and `TUNIT_SCOPED_TRACE(msg)`
- **Custom Evaluators**: Support for custom predicates and evaluation logic
- **Container Algorithm Testing**: Integration with STL containers for argument-dependent lookup (ADL)
- **Performance Testing**: Lightweight framework suitable for performance-critical testing
- **Debug Support**: Enhanced debugging capabilities with trace information
### (TODO)

- **Compile-Time Assertions**: `assert` statements are evaluated at compile time when possible, enabling static verification of predicates and values.
- **Runtime Expectations**: `expect` statements are always evaluated at runtime, allowing for dynamic checks and flexible test flows.

## Quick Start

Get started with TUnit in just a few lines:
```cpp
#include "tUnit.h"
// Define your tests inside an anonymous namespace
namespace {

namespace pred = tUnit::predicates;
auto &suite = tUnit::Orchestrator::instance().get_suite("foo");

    void bar() {
        auto& suite = tUnit::Orchestrator::instance().get_suite("Basic Tests");
        auto& test = suite.get_test("Simple Assertions");

        // Basic assertions
        test.assert("equality check", 5, pred::is_equal{}, 5);
        test.expect("range validation", pred::is_in_range{}(7, 1, 10), true);
    }
    bar()
}

int main(int argc, char* argv[]) {
    // Test suites are automatically registered and executed
    tUnit::Orchestrator::instance().parse_args(argc, argv);
    tUnit::Orchestrator::instance().print_summary();
    tUnit::Orchestrator::instance().write_xml_output();
    return tUnit::Orchestrator::instance().all_tests_passed() ? 0 : 1;
}
```

## Testing Patterns

TUnit supports multiple testing approaches:

**Assert Style** (predicate-based comparisons):
```cpp
test.assert("description", value1, predicate, value2);
```

**Expect Style** (boolean expressions or single value checks):
```cpp
test.expect("description", boolean_expression, expected_result);
// Or, to check a single value is true:
test.expect("description", value); // same function, expected default == true
```

**Container Testing**:
```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5};
test.assert("contains element", numbers, pred::contains_element{}, 3);
test.expect("all positive", pred::all_elements_satisfy{}(numbers, pred::is_positive{}), true);
```

## Build Integration

### CMake Integration
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

### Example Test Output
```
=== Test Summary ===
Total assertions: 152
Passed: 151
Failed: 1
===================
```

## Requirements

- **C++17 or later**: Modern C++ features for type safety and performance

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
