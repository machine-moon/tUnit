#include <string>
#include <string_view>

#include "tsuite.h"

namespace {
namespace pred = tunit::predicates;
auto &suite = tunit::Runner::get_suite("Common Predicates");

void test_basic_comparisons() {
  auto &mytest = suite.get_test("Basic Comparisons");

  mytest.assert("5 is_equal 5", 5, pred::is_equal{}, 5);

  suite.test("5 is_equal 5", 5, pred::is_equal{}, 5);
  suite.test("10 is_greater 5", 10, pred::is_greater{}, 5);
  suite.test("3 is_less 7", 3, pred::is_less{}, 7);
  suite.test("5 is_greater_equal 5", 5, pred::is_greater_equal{}, 5);
  suite.test("3 is_less_equal 7", 3, pred::is_less_equal{}, 7);
  suite.test("5 is_not_equal 3", 5, pred::is_not_equal{}, 3);
}

void test_string_predicates() {
  auto &suite = tunit::Runner::get_suite("Common Predicates");

  suite.test("hello contains_substring ell", std::string("hello"),
             pred::contains_substring{}, std::string("ell"));
  suite.test("hello starts_with he", std::string("hello"), pred::starts_with{},
             std::string("he"));
  suite.test("hello ends_with lo", std::string("hello"), pred::ends_with{},
             std::string("lo"));
}

void test_string_predicates_negative() {
  auto &suite = tunit::Runner::get_suite("Common Predicates");

  // Failed string cases (should fail)
  suite.test("hello does_not_contain xyz", std::string("hello"),
             pred::contains_substring{}, std::string("xyz"), false);
  suite.test("hello does_not_start_with bye", std::string("hello"),
             pred::starts_with{}, std::string("bye"), false);
  suite.test("hello does_not_end_with abc", std::string("hello"),
             pred::ends_with{}, std::string("abc"), false);
}

void test_range_predicates() {
  auto &suite = tunit::Runner::get_suite("Common Predicates");

  // Range predicates (3-arg predicates)
  suite.test("5 is_in_range 1-10", pred::is_in_range{}(5, 1, 10));
  suite.test("15 is_out_of_range 1-10", pred::is_out_of_range{}(15, 1, 10));
  suite.test("0 is_in_range 0-10", pred::is_in_range{}(0, 0, 10));
  suite.test("-5 is_out_of_range 1-10", pred::is_out_of_range{}(-5, 1, 10));
}

void test_numeric_predicates() {
  auto &suite = tunit::Runner::get_suite("Common Predicates");

  // Numeric predicates (single value)
  suite.test("4 is_even", pred::is_even{}(4));
  suite.test("5 is_odd", pred::is_odd{}(5));
  suite.test("10 is_positive", pred::is_positive{}(10));
  suite.test("-5 is_negative", pred::is_negative{}(-5));
  suite.test("0 is_zero", pred::is_zero{}(0));
}

void test_two_value_predicates() {
  auto &suite = tunit::Runner::get_suite("Common Predicates");

  // Two-value predicates using binary operators
  suite.test("4 and 6 are_both_even", 4, pred::is_even{}, 6);
  suite.test("3 and 7 are_both_odd", 3, pred::is_odd{}, 7);
  suite.test("5 and 15 are_both_positive", 5, pred::is_positive{}, 15);
  suite.test("-3 and -7 are_both_negative", -3, pred::is_negative{}, -7);
  suite.test("0 and 0 are_both_zero", 0, pred::is_zero{}, 0);
}

void test_edge_cases() {
  auto &suite = tunit::Runner::get_suite("Common Predicates");

  // Edge cases and negative tests
  suite.test("0 is_even", pred::is_even{}(0));
  suite.test("-4 is_even", pred::is_even{}(-4));
  suite.test("Large number is_positive", pred::is_positive{}(1000000));
  suite.test("3 is_not_even", pred::is_even{}(3), false);
  suite.test("4 is_not_odd", pred::is_odd{}(4), false);
  suite.test("-1 is_not_positive", pred::is_positive{}(-1), false);
}

// Static initialization to run tests
struct CommonPredicatesTestRunner {
  CommonPredicatesTestRunner() {
    test_basic_comparisons();
    test_string_predicates();
    test_string_predicates_negative();
    test_range_predicates();
    test_numeric_predicates();
    test_two_value_predicates();
    test_edge_cases();
  }
};

static CommonPredicatesTestRunner runner;

} // anonymous namespace
