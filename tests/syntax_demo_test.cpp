#include <string>
#include <vector>

#include "tunit.h"

namespace {
namespace pred = tunit::predicates;

void test_basic_numeric_comparisons() {
  auto& suite = tunit::Runner::get_suite("Syntax Demo");

  // ********************** val1 is_predicate val2 Syntax Demo **********************

  // Basic numeric comparisons - showcasing the natural language style
  suite.test("5 is_equal 5", 5, pred::is_equal{}, 5);
  suite.test("10 is_greater 5", 10, pred::is_greater{}, 5);
  suite.test("3 is_less 7", 3, pred::is_less{}, 7);
  suite.test("5 is_greater_equal 5", 5, pred::is_greater_equal{}, 5);
  suite.test("3 is_less_equal 7", 3, pred::is_less_equal{}, 7);
  suite.test("5 is_not_equal 3", 5, pred::is_not_equal{}, 3);
}

void test_string_operations() {
  auto &suite = tunit::Runner::get_suite("Syntax Demo");

  // String operations using the same syntax
  suite.test("hello contains_substring ell", std::string("hello"), pred::contains_substring{}, std::string("ell"));
  suite.test("hello starts_with he", std::string("hello"), pred::starts_with{}, std::string("he"));
  suite.test("world ends_with rld", std::string("world"), pred::ends_with{}, std::string("rld"));
}

void test_container_operations() {
  auto &suite = tunit::Runner::get_suite("Syntax Demo");

  // Container operations
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  std::vector<int> subset = {2, 4};
  std::vector<int> permuted = {5, 4, 3, 2, 1};

  suite.test("numbers contains_element 3", numbers, pred::contains_element{}, 3);
  suite.test("numbers contains_all_elements subset", numbers, pred::contains_all_elements{}, subset);
  suite.test("numbers is_permutation_of permuted", numbers, pred::is_permutation_of{}, permuted);
}

void test_single_parameter_predicates() {
  auto &suite = tunit::Runner::get_suite("Syntax Demo");

  // ********************** Single Parameter Predicates **********************
  // All single parameter tests use the boolean test method for consistency

  suite.test("4 is_even", pred::is_even{}(4));
  suite.test("5 is_odd", pred::is_odd{}(5));
  suite.test("10 is_positive", pred::is_positive{}(10));
  suite.test("-5 is_negative", pred::is_negative{}(-5));
  suite.test("0 is_zero", pred::is_zero{}(0));
  suite.test("3 is_not_even", pred::is_even{}(3), false);  // expect something

  // Container single-parameter predicates
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  suite.test("numbers is_not_empty", pred::is_not_empty{}(numbers));
  suite.test("numbers is_sorted", pred::is_sorted{}(numbers));
  suite.test("numbers is_unique", pred::is_unique{}(numbers));
}

void test_complex_predicates() {
  auto &suite = tunit::Runner::get_suite("Syntax Demo");

  // ********************** Complex Predicates in Direct Form **********************
  // For complex predicates that don't fit the v1 predicate v2 pattern

  suite.test("4 satisfies positive_and_even", pred::is_positive{}(4) && pred::is_even{}(4));

  std::vector<int> nums = {1, 2, 3, 4, 5};
  suite.test("numbers all_elements_satisfy positive", pred::all_elements_satisfy{}(nums, pred::is_positive{}));
}

void test_range_predicates() {
  auto &suite = tunit::Runner::get_suite("Syntax Demo");

  // ********************** Range Predicates (3+ arguments) **********************
  // Multi-argument predicates in direct form

  suite.test("5 is_in_range 1-10", pred::is_in_range{}(5, 1, 10));
  suite.test("15 is_out_of_range 1-10", pred::is_out_of_range{}(15, 1, 10));
}

void test_expectations() {
  auto &suite = tunit::Runner::get_suite("Syntax Demo");

  // ********************** Expectations **********************
  // Testing failure cases with expected=true

  suite.test("5 is_not 10", 5, pred::is_equal{}, 10, false);
  suite.test("10 is_not_less 5", 10, pred::is_less{}, 5, false);
  suite.test("5 is_equal 5 (expected true)", 5, pred::is_equal{}, 5, true);
  suite.test("hello contains_substring ell (expected true)", std::string("hello"), pred::contains_substring{}, std::string("ell"), true);
}

void test_mixed_type_comparisons() {
  auto &suite = tunit::Runner::get_suite("Syntax Demo");

  // ********************** Mixed Type Comparisons **********************
  suite.test("5 is_equal 5.0", 5, pred::is_equal{}, 5.0);
  suite.test("char_A is_equal 65", 'A', pred::is_equal{}, 65);
}

void test_string_view_compatibility() {
  auto &suite = tunit::Runner::get_suite("Syntax Demo");

  // ********************** String View Compatibility **********************
  std::string_view sv = "hello";
  suite.test("string_view contains_substring ell", sv, pred::contains_substring{}, std::string("ell"));
}

struct TestRunner {
  TestRunner() {
    test_basic_numeric_comparisons();
    test_string_operations();
    test_container_operations();
    test_single_parameter_predicates();
    test_complex_predicates();
    test_range_predicates();
    test_expectations();
    test_mixed_type_comparisons();
    test_string_view_compatibility();
  }
};

static TestRunner runner;

} // anonymous namespace
