# C++ Evaluator Project

## Overview
The C++ Evaluator Project is a robust, header-only library designed to provide a flexible framework for evaluating predicates on any type of objects. The project features a powerful `Evaluator` class that supports comparing ANY object with ANY predicate using modern C++ features including variadic templates, `std::function`, and type deduction.

## Key Features
- **Header-only library**: Easy integration with no compilation dependencies
- **Variadic template support**: Create evaluators with any number of arguments
- **Type-safe comparisons**: Compile-time type checking with runtime flexibility
- **Comprehensive predicate library**: Built-in predicates for common operations
- **Logical combinators**: Combine predicates with AND, OR, NOT operations
- **Container evaluation**: Evaluate predicates across collections
- **Factory functions**: Easy creation with automatic type deduction

## Project Structure
```
cpp-evaluator-project
├── src
│   ├── main.cpp            # Demonstration program
│   ├── evaluator.cpp       # Empty (header-only implementation)
│   └── evaluator.h         # Main header-only library
├── include
│   └── predicates.h        # Comprehensive predicate functions
├── tests
│   ├── simple_test.cpp     # Standalone unit tests (32 test cases)
│   └── test_evaluator.cpp  # GoogleTest unit tests
├── CMakeLists.txt          # CMake configuration with optional GoogleTest
└── README.md               # Project documentation
```

## Setup Instructions

### Prerequisites
- C++14 compatible compiler (GCC 5+, Clang 3.8+, MSVC 2015+)
- CMake 3.10+ (for building)
- GoogleTest (optional, for advanced tests)

### Building the Project

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd cpp-evaluator-project
   ```

2. **Build with CMake**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the demonstration**:
   ```bash
   ./evaluator_demo
   ```

4. **Run the tests**:
   ```bash
   # Run simple standalone tests
   ./simple_test
   
   # Run GoogleTest tests (if available)
   ./test_evaluator
   ```

## Core Usage

### Basic Evaluation

```cpp
#include "evaluator.h"
#include "predicates.h"

// Create evaluator with factory function (automatic type deduction)
auto eval = make_evaluator(Equals{}, 42, 42);
bool result = eval.evaluate(); // true

// Manual creation with explicit types
Evaluator<int, int> eval2(Equals{}, 10, 20);
bool result2 = eval2.evaluate(); // false
```

### Using Built-in Predicates

```cpp
// Numeric comparisons
auto gt_eval = make_evaluator(Greater{}, 10, 5);        // 10 > 5 = true
auto le_eval = make_evaluator(LessEqual{}, 3, 7);       // 3 <= 7 = true

// String operations
auto str_eval = make_evaluator(Equals{}, 
                              std::string("hello"), 
                              std::string("hello"));     // true

// Container operations
std::vector<int> vec = {1, 2, 3, 4, 5};
auto size_eval = make_evaluator(predicates::has_size<int>, vec, 5); // true
auto empty_eval = make_evaluator(predicates::container_is_empty<int>, 
                                std::vector<int>{}); // true
```

### Logical Combinations

```cpp
// AND combination
auto gt_eval = make_evaluator(Greater{}, 10, 5);
auto lt_eval = make_evaluator(Less{}, 10, 15);
auto and_eval = make_and(gt_eval, lt_eval);            // (10 > 5) && (10 < 15) = true

// OR combination  
auto eq1 = make_evaluator(Equals{}, 5, 5);
auto eq2 = make_evaluator(Equals{}, 3, 7);
auto or_eval = make_or(eq1, eq2);                      // (5 == 5) || (3 == 7) = true

// NOT combination
auto not_eval = make_not(eq2);                         // !(3 == 7) = true
```

### Container Evaluation

```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// Count elements meeting criteria
auto gt5 = make_evaluator(Greater{}, 0, 5); // Template for comparison
int count = count_if_predicate(numbers, [](int x) { return x > 5; }); // 5

// Check if all elements meet criteria
bool all_positive = all_of_predicate(numbers, [](int x) { return x > 0; }); // true

// Check if any element meets criteria  
bool any_even = any_of_predicate(numbers, [](int x) { return x % 2 == 0; }); // true

// Check if no elements meet criteria
bool none_negative = none_of_predicate(numbers, [](int x) { return x < 0; }); // true
```

## Advanced Usage

### Custom Predicates

```cpp
// Custom predicate struct
struct IsEven {
    template<typename T>
    bool operator()(const T& value) const {
        return value % 2 == 0;
    }
};

// Custom predicate function
auto is_in_range = [](int value, int min, int max) {
    return value >= min && value <= max;
};

// Usage
auto even_eval = make_evaluator(IsEven{}, 8);           // true
auto range_eval = make_evaluator(is_in_range, 5, 1, 10); // true
```

### Complex Logical Expressions

```cpp
// (x > 5 AND x < 10) OR x == 0
auto gt5 = make_evaluator(Greater{}, 7, 5);
auto lt10 = make_evaluator(Less{}, 7, 10);
auto eq0 = make_evaluator(Equals{}, 7, 0);

auto range_check = make_and(gt5, lt10);
auto complex_eval = make_or(range_check, eq0);         // true
```

### Working with Different Types

```cpp
// Floating point comparisons
auto float_eval = make_evaluator(GreaterEqual{}, 3.14, 3.0);

// String comparisons
auto str_eval = make_evaluator(Less{}, 
                              std::string("apple"), 
                              std::string("banana"));

// Custom objects (requires appropriate operators)
struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

auto point_eval = make_evaluator(Equals{}, Point{1,2}, Point{1,2});
```

## Available Predicates

### Comparison Predicates
- `Equals{}` - Equality comparison (`==`)
- `NotEquals{}` - Inequality comparison (`!=`)
- `Greater{}` - Greater than comparison (`>`)
- `Less{}` - Less than comparison (`<`)
- `GreaterEqual{}` - Greater than or equal comparison (`>=`)
- `LessEqual{}` - Less than or equal comparison (`<=`)

### Numeric Predicates
- `predicates::is_positive<T>(value)` - Check if value > 0
- `predicates::is_negative<T>(value)` - Check if value < 0
- `predicates::is_zero<T>(value)` - Check if value == 0
- `predicates::is_even<T>(value)` - Check if value is even
- `predicates::is_odd<T>(value)` - Check if value is odd
- `predicates::in_range<T>(value, min, max)` - Check if value is in range

### String Predicates
- `predicates::starts_with(str, prefix)` - Check if string starts with prefix
- `predicates::ends_with(str, suffix)` - Check if string ends with suffix
- `predicates::contains_substring(str, substr)` - Check if string contains substring
- `predicates::is_empty_string(str)` - Check if string is empty

### Container Predicates
- `predicates::has_size<T>(container, expected_size)` - Check container size
- `predicates::container_is_empty<T>(container)` - Check if container is empty
- `predicates::contains_element<T>(container, element)` - Check if container contains element

## Testing

The project includes comprehensive testing:

### Simple Tests (Standalone)
32 test cases covering all functionality:
```bash
./simple_test
```

### GoogleTest Tests (Optional)
Advanced test framework integration:
```bash
./test_evaluator
```

### Test Coverage
- Basic predicate evaluation
- Logical combinations (AND, OR, NOT)
- Container operations
- Type safety verification
- Edge cases and error conditions

## API Reference

### Evaluator Class
```cpp
template<typename... Args>
class Evaluator {
    // Constructor
    template<typename Predicate>
    explicit Evaluator(Predicate&& pred, Args... args);
    
    // Evaluation
    bool evaluate() const;
    
    // Predicate modification
    template<typename Predicate>
    void setPredicate(Predicate&& pred);
};
```

### Factory Functions
```cpp
// Create evaluator with type deduction
template<typename Predicate, typename... Args>
auto make_evaluator(Predicate&& pred, Args&&... args);

// Logical combinators
template<typename Eval1, typename Eval2>
auto make_and(const Eval1& eval1, const Eval2& eval2);

template<typename Eval1, typename Eval2>
auto make_or(const Eval1& eval1, const Eval2& eval2);

template<typename Eval>
auto make_not(const Eval& eval);
```

### Container Functions
```cpp
template<typename Container, typename Predicate>
size_t count_if_predicate(const Container& container, Predicate pred);

template<typename Container, typename Predicate>
bool all_of_predicate(const Container& container, Predicate pred);

template<typename Container, typename Predicate>
bool any_of_predicate(const Container& container, Predicate pred);

template<typename Container, typename Predicate>
bool none_of_predicate(const Container& container, Predicate pred);
```

## Performance Considerations

- **Header-only**: No runtime linking overhead
- **Template-based**: Compile-time optimization opportunities
- **std::function**: Small runtime overhead for flexibility
- **Memory efficient**: Minimal memory footprint
- **Cache-friendly**: Value semantics where possible

## Best Practices

1. **Use factory functions**: `make_evaluator()` provides better type deduction
2. **Prefer built-in predicates**: They're optimized and well-tested
3. **Combine logically**: Use `make_and()`, `make_or()`, `make_not()` for complex logic
4. **Consider const**: Most operations work with const objects
5. **Template specialization**: Create specialized predicates for custom types

## Contributing

Contributions are welcome! Areas for improvement:

- Additional predicate functions
- Performance optimizations
- Extended container support
- More comprehensive testing
- Documentation improvements

### Development Guidelines
1. Maintain header-only design
2. Ensure C++14 compatibility
3. Add comprehensive tests for new features
4. Follow existing code style
5. Update documentation

## License

[Specify your license here]

## Changelog

### Version 2.0 (Current)
- Complete redesign with variadic templates
- Header-only implementation
- Comprehensive predicate library
- Logical combination support
- Container evaluation functions
- Factory functions with type deduction
- 32 comprehensive test cases

### Version 1.0 (Legacy)
- Basic evaluator with simple predicates
- Template-based design
- Limited functionality# tunit
# tunit
# tunit
