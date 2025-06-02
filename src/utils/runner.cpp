#include "../../include/utils/runner.h"
#include <cassert>

namespace tunit {

std::size_t& Runner::get_total_passes() {
  static std::size_t total_passes = 0;
  return total_passes;
}

std::size_t& Runner::get_total_fails() {
  static std::size_t total_fails = 0;
  return total_fails;
}

std::vector<std::string>& Runner::get_failed_tests() {
  static std::vector<std::string> failed_tests;
  return failed_tests;
}

Runner::Runner(std::string suite_name) : suite_name_(std::move(suite_name)), suite_passes_(0), suite_fails_(0) {}

void Runner::record_result(const std::string& test_name, bool passed) {
  if (passed) {
    get_total_passes()++;
    suite_passes_++;
  } else {
    get_total_fails()++;
    suite_fails_++;
    get_failed_tests().push_back(suite_name_ + ": " + test_name);
  }
}

Runner& Runner::get_suite(const std::string& suite_name) {
  static std::vector<std::unique_ptr<Runner>> suites;

  for (auto& suite : suites) {
    if (suite->suite_name_ == suite_name) {
      return *suite;
    }
  }

  suites.push_back(std::unique_ptr<Runner>(new Runner(suite_name)));
  return *suites.back();
}

void Runner::test(const std::string& test_name, bool condition, bool expected) {
  assert(condition == expected);
  record_result(test_name, condition == expected);
}

bool Runner::all_tests_passed() { return get_total_fails() == 0 && get_total_passes() > 0; }

void Runner::reset_global_counters() {
  get_total_passes() = 0;
  get_total_fails() = 0;
  get_failed_tests().clear();
}

std::size_t Runner::get_total_passes_count() { return get_total_passes(); }

std::size_t Runner::get_total_fails_count() { return get_total_fails(); }

}  // namespace tunit
