#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../evaluator.h"
#include "colors.h"
#include "release_asserts.h"

namespace tunit {

class TestRunner {
 private:
  std::string suite_name_;
  int suite_passes_;
  int suite_fails_;

  static int& get_total_passes();
  static int& get_total_fails();
  static std::vector<std::string>& get_failed_tests();

  TestRunner(std::string suite_name);
  void record_result(const std::string& test_name, bool passed);

 public:
  static TestRunner& get_suite(const std::string& suite_name);

  template <typename T, typename P, typename U>
  void test(const std::string& test_name, const T& lhs, P pred, const U& rhs, bool expected = true);
  void test(const std::string& test_name, bool condition, bool expected = true);

  void suite_header() const;
  void suite_summary() const;
  static void global_summary();

  static bool all_tests_passed();
  static void reset_global_counters();
  static int get_total_passes_count();
  static int get_total_fails_count();
};

// Template implementation
template <typename T, typename P, typename U>
void TestRunner::test(const std::string& test_name, const T& lhs, P pred, const U& rhs, bool expected) {
  Evaluator evaluator(lhs, rhs, pred);
  bool result = evaluator();
  assert(result == expected);
  record_result(test_name, result == expected);
}
}  // namespace tunit
