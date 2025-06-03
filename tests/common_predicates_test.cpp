#include "tunit.h"
#include <iostream>
#include <string>
#include <string_view>

namespace {
namespace pred = tunit::predicates;

auto &suite = tunit::Orchestrator::instance().get_suite("Common Predicates");

void test_basic_comparisons() {
  auto &test = suite.get_test("Basic Comparisons");
  test.assert("5 is_equal 5", 5, pred::is_equal{}, 5);
  test.assert("10 is_greater 5", 10, pred::is_greater{}, 5);
  test.assert("3 is_less 7", 3, pred::is_less{}, 7);
  test.assert("5 is_greater_equal 5", 5, pred::is_greater_equal{}, 5);
  test.assert("3 is_less_equal 7", 3, pred::is_less_equal{}, 7);
  test.assert("5 is_not_equal 3", 5, pred::is_not_equal{}, 3);
}

void test_cstring_predicates() {
  auto &test = suite.get_test("CString Predicates");
  const char *hello = "hello";
  const char *ell = "ell";
  const char *he = "he";
  const char *lo = "lo";
  test.assert("hello contains_substring ell", hello, pred::contains_substring{}, ell);
  test.assert("hello starts_with he", hello, pred::starts_with{}, he);
  test.assert("hello ends_with lo", hello, pred::ends_with{}, lo);
}

void test_string_predicates() {
  auto &test = suite.get_test("String Predicates");
  test.assert("hello contains_substring ell", std::string("hello"), pred::contains_substring{}, std::string("ell"));
  test.assert("hello starts_with he", std::string("hello"), pred::starts_with{}, std::string("he"));
  test.assert("hello ends_with lo", std::string("hello"), pred::ends_with{}, std::string("lo"));
}

void test_range_predicates() {
  auto &test = suite.get_test("Range Predicates");
  test.assert("5 is_greater 1", 5, pred::is_greater{}, 1);
  test.assert("5 is_less 10", 5, pred::is_less{}, 10);
  test.assert("0 is_greater_equal 0", 0, pred::is_greater_equal{}, 0);
  test.assert("0 is_less_equal 10", 0, pred::is_less_equal{}, 10);
}

void test_numeric_predicates() {
  auto &test = suite.get_test("Numeric Predicates");
  test.assert("4 is_equal 4", 4, pred::is_equal{}, 4);
  test.assert("5 is_not_equal 4", 5, pred::is_not_equal{}, 4);
  test.assert("10 is_greater 0", 10, pred::is_greater{}, 0);
  test.assert("-5 is_less 0", -5, pred::is_less{}, 0);
  test.assert("0 is_equal 0", 0, pred::is_equal{}, 0);
}

void test_two_value_predicates() {
  auto &test = suite.get_test("Two Value Predicates");
  test.assert("4 is_equal 4", 4, pred::is_equal{}, 4);
  test.assert("3 is_equal 3", 3, pred::is_equal{}, 3);
  test.assert("5 is_equal 5", 5, pred::is_equal{}, 5);
  test.assert("-3 is_equal -3", -3, pred::is_equal{}, -3);
  test.assert("0 is_equal 0", 0, pred::is_equal{}, 0);
}

void test_edge_cases() {
  auto &test = suite.get_test("Edge Cases");
  test.assert("0 is_equal 0", 0, pred::is_equal{}, 0);
  test.assert("-4 is_equal -4", -4, pred::is_equal{}, -4);
  test.assert("Large number is_equal itself", 1000000, pred::is_equal{}, 1000000);
  test.assert("3 is_not_equal 2", 3, pred::is_not_equal{}, 2);
  test.assert("4 is_not_equal 5", 4, pred::is_not_equal{}, 5);
  test.assert("-1 is_less 0", -1, pred::is_less{}, 0);
}

// Static initialization to run tests
struct CommonPredicatesTestRunner {
  CommonPredicatesTestRunner() {
    test_basic_comparisons();
    test_string_predicates();
    test_cstring_predicates();
    test_range_predicates();
    test_numeric_predicates();
    test_two_value_predicates();
    test_edge_cases();
  }
};

static CommonPredicatesTestRunner runner;

} // anonymous namespace
