#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../evaluator.h"
#include "release_asserts.h"

namespace tunit {

class Runner {
 private:
  std::string suite_name_;
  std::size_t suite_passes_;
  std::size_t suite_fails_;

  static std::size_t& get_total_passes();
  static std::size_t& get_total_fails();
  static std::vector<std::string>& get_failed_tests();

  Runner(std::string suite_name);
  void record_result(const std::string& test_name, bool passed);

 public:
  static Runner& get_suite(const std::string& suite_name);

  template <typename T, typename P, typename U>
  void test(const std::string& test_name, const T& lhs, P pred, const U& rhs, bool expected = true);
  void test(const std::string& test_name, bool condition, bool expected = true);

  static bool all_tests_passed();
  static void reset_global_counters();
  static std::size_t get_total_passes_count();
  static std::size_t get_total_fails_count();
};

// Template implementation
template <typename T, typename P, typename U>
void Runner::test(const std::string& test_name, const T& lhs, P pred, const U& rhs, bool expected) {
  Evaluator evaluator(lhs, rhs, pred);
  bool result = evaluator();
  assert(result == expected);
  record_result(test_name, result == expected);
}
}  // namespace tunit
