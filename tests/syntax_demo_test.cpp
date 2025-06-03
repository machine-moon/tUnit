#include "tunit.h"
#include <string>
#include <vector>

namespace {
namespace pred = tunit::predicates;

auto &suite = tunit::Orchestrator::instance().get_suite("Syntax Demo");

void test_basic_numeric_comparisons() {
  auto& test = suite.get_test("Basic Numeric Comparisons");
  test.assert("5 is_equal 5", 5, pred::is_equal{}, 5);
  test.assert("10 is_greater 5", 10, pred::is_greater{}, 5);
  test.assert("3 is_less 7", 3, pred::is_less{}, 7);
  test.assert("5 is_greater_equal 5", 5, pred::is_greater_equal{}, 5);
  test.assert("3 is_less_equal 7", 3, pred::is_less_equal{}, 7);
  test.assert("5 is_not_equal 3", 5, pred::is_not_equal{}, 3);
}

void test_string_operations() {
  auto &test = suite.get_test("String Operations");
  test.assert("hello contains_substring ell", std::string("hello"), pred::contains_substring{}, std::string("ell"));
  test.assert("hello starts_with he", std::string("hello"), pred::starts_with{}, std::string("he"));
  test.assert("world ends_with rld", std::string("world"), pred::ends_with{}, std::string("rld"));
}

void test_container_operations() {
  auto &test = suite.get_test("Container Operations");
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  std::vector<int> subset = {2, 4};
  std::vector<int> permuted = {5, 4, 3, 2, 1};
  test.assert("numbers contains_element 3", numbers, pred::contains_element{}, 3);
  test.assert("numbers contains_all_elements subset", numbers, pred::contains_all_elements{}, subset);
  test.assert("numbers is_permutation_of permuted", numbers, pred::is_permutation_of{}, permuted);
}

void test_single_parameter_predicates() {
  auto &test = suite.get_test("Single Parameter Predicates");
  test.expect("4 is_even", pred::is_even{}(4), true);
  test.expect("5 is_odd", pred::is_odd{}(5), true);
  test.expect("10 is_positive", pred::is_positive{}(10), true);
  test.expect("-5 is_negative", pred::is_negative{}(-5), true);
  test.expect("0 is_zero", pred::is_zero{}(0), true);
  test.expect("3 is_not_even", pred::is_even{}(3), false);
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  test.expect("numbers is_not_empty", pred::is_not_empty{}(numbers), true);
  test.expect("numbers is_sorted", pred::is_sorted{}(numbers), true);
  test.expect("numbers is_unique", pred::is_unique{}(numbers), true);
}

void test_complex_predicates() {
  auto &test = suite.get_test("Complex Predicates");
  test.expect("4 satisfies positive_and_even", pred::is_positive{}(4) && pred::is_even{}(4), true);
  std::vector<int> nums = {1, 2, 3, 4, 5};
  test.expect("numbers all_elements_satisfy positive", pred::all_elements_satisfy{}(nums, pred::is_positive{}), true);
}

void test_range_predicates() {
  auto &test = suite.get_test("Range Predicates");
  test.expect("5 is_in_range 1-10", pred::is_in_range{}(5, 1, 10), true);
  test.expect("15 is_out_of_range 1-10", pred::is_out_of_range{}(15, 1, 10), true);
}

void test_expectations() {
  auto &test = suite.get_test("Expectations");
  test.assert("5 is_not 10", 5, pred::is_equal{}, 10);
  test.expect("10 is_not_less 5", pred::is_less{}(10, 5), false);
  test.assert("5 is_equal 5 (expected true)", 5, pred::is_equal{}, 5);
  test.assert("hello contains_substring ell (expected true)", std::string("hello"), pred::contains_substring{}, std::string("ell"));
}

void test_mixed_type_comparisons() {
  auto &test = suite.get_test("Mixed Type Comparisons");
  test.assert("5(int) is_equal 5(double)", 5, pred::is_equal{}, 5.0);
  test.assert("5(double) is_equal 5(int)", 5.0, pred::is_equal{}, 5);
}

struct SyntaxDemoTestRunner {
  SyntaxDemoTestRunner() {
    test_basic_numeric_comparisons();
    test_string_operations();
    test_container_operations();
    test_single_parameter_predicates();
    test_complex_predicates();
    test_range_predicates();
    test_expectations();
    test_mixed_type_comparisons();
  }
};

static SyntaxDemoTestRunner runner;

} // anonymous namespace
