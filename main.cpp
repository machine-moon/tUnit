#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

#include "include/tunit.h"

// test 1
template <typename T>
bool is_equal_func(const T lhs, const T rhs) {
  return lhs == rhs;
}

// Test 2: Namespaced template function predicate
namespace someSpace {
template <typename T>
bool is_greater_equal(const T& lhs, const T& rhs) {
  return std::greater_equal<T>{}(lhs, rhs);
}
}  // namespace someSpace

// Test 3: Function object (functor) predicate
struct IsEqualFunctor {
  template <typename T>
  bool operator()(const T& lhs, const T& rhs) const {
    return lhs == rhs;
  }
};

// Test 4: Lambda expression predicate
auto is_equal_lambda = [](const auto& lhs, const auto& rhs) { return tunit::predicates::is_equal{}(lhs, rhs); };

int main() {
  // Test 1: Template function predicate
  tunit::Evaluator test1(1, 1, is_equal_func);

  // Test 2: Namespaced template function predicate (ugly implicit deduction)
  tunit::Evaluator test2(1, 1, someSpace::is_greater_equal<int>);

  // To avoid implicit function predicates:
  int a{}, b{};
  tunit::Evaluator testDecla(a, b, someSpace::is_greater_equal<decltype(a)>);  // better but still ugly

  // Test 3: Function object (functor) predicate - best option
  tunit::Evaluator test3(1, 1, IsEqualFunctor{});

  // Test 4: Lambda expression predicate
  tunit::Evaluator test4(1, 1, is_equal_lambda);

  // Test 5: Inline lambda predicate
  tunit::Evaluator test5(1, 1, [](const auto& lhs, const auto& rhs) { return lhs == rhs; });

  // Output test results
  std::cout << "\ntest1 (template function): " << test1();
  std::cout << "\ntest2 (namespace::template_function): " << test2();
  std::cout << "\ntest3 (functor: recommended option): " << test3();
  std::cout << "\ntest4 (lambda wrapper around namespace::template_func): " << test4();
  std::cout << "\ntest5 (inline lambda): " << test5.evaluate();
  std::cout << std::endl;

  return 0;
}