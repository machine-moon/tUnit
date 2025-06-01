#include "../../include/utils/test_runner.h"

#include "../../include/utils/colors.h"

namespace tunit {

// Static member functions for global counters
int& TestRunner::get_total_passes() {
  static int total_passes = 0;
  return total_passes;
}

int& TestRunner::get_total_fails() {
  static int total_fails = 0;
  return total_fails;
}

std::vector<std::string>& TestRunner::get_failed_tests() {
  static std::vector<std::string> failed_tests;
  return failed_tests;
}

// Constructor
TestRunner::TestRunner(std::string suite_name) : suite_name_(std::move(suite_name)), suite_passes_(0), suite_fails_(0) {}

// Core functionality
void TestRunner::record_result(const std::string& test_name, bool passed) {
  if (passed) {
    get_total_passes()++;
    suite_passes_++;
    std::cout << Colors::colorize("[PASS]", Colors::GREEN + Colors::BOLD) << " " << Colors::colorize(suite_name_, Colors::CYAN) << ": " << test_name << "\n";
  } else {
    get_total_fails()++;
    suite_fails_++;
    get_failed_tests().push_back(suite_name_ + ": " + test_name);
    std::cout << Colors::colorize("[FAIL]", Colors::RED + Colors::BOLD) << " " << Colors::colorize(suite_name_, Colors::CYAN) << ": " << test_name << "\n";
  }
}

TestRunner& TestRunner::get_suite(const std::string& suite_name) {
  static std::vector<std::unique_ptr<TestRunner>> suites;

  // Find existing suite
  for (auto& suite : suites) {
    if (suite->suite_name_ == suite_name) {
      return *suite;
    }
  }

  // Create new suite
  suites.push_back(std::unique_ptr<TestRunner>(new TestRunner(suite_name)));
  return *suites.back();
}

void TestRunner::test(const std::string& test_name, bool condition, bool expected) {
  assert(condition == expected);
  record_result(test_name, condition == expected);
}

// Suite reporting
void TestRunner::suite_header() const { std::cout << Colors::colorize("--- Running " + suite_name_ + " ---", Colors::CYAN + Colors::BOLD) << "\n"; }

void TestRunner::suite_summary() const {
  std::cout << Colors::colorize("--- " + suite_name_ + " Summary ---", Colors::CYAN + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Passed: ", Colors::GREEN) << Colors::colorize(std::to_string(suite_passes_), Colors::GREEN + Colors::BOLD) << ", " << Colors::colorize("Failed: ", Colors::RED) << Colors::colorize(std::to_string(suite_fails_), Colors::RED + Colors::BOLD) << "\n\n";
}

// Global reporting
void TestRunner::global_summary() {
  std::string separator = std::string(60, '=');
  std::cout << "\n" << Colors::colorize(separator, Colors::BLUE) << "\n";
  std::cout << Colors::colorize("GLOBAL TEST SUMMARY", Colors::BLUE + Colors::BOLD) << "\n";
  std::cout << Colors::colorize(separator, Colors::BLUE) << "\n";

  std::cout << Colors::colorize("Total Passes: ", Colors::GREEN) << Colors::colorize(std::to_string(get_total_passes()), Colors::GREEN + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Total Fails:  ", Colors::RED) << Colors::colorize(std::to_string(get_total_fails()), Colors::RED + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Total Tests:  ", Colors::WHITE) << Colors::colorize(std::to_string(get_total_passes() + get_total_fails()), Colors::WHITE + Colors::BOLD) << "\n";

  if (get_total_fails() > 0) {
    std::cout << "\n" << Colors::colorize("Failed Tests:", Colors::RED + Colors::BOLD) << "\n";
    for (const auto& test : get_failed_tests()) {
      std::cout << "  " << Colors::colorize("-", Colors::RED) << " " << Colors::colorize(test, Colors::DIM) << "\n";
    }
  }

  // Calculate and display pass rate
  double pass_rate = get_total_passes() + get_total_fails() > 0 ? (static_cast<double>(get_total_passes()) / (get_total_passes() + get_total_fails())) * 100.0 : 0.0;

  std::string pass_rate_str = std::to_string(pass_rate).substr(0, std::to_string(pass_rate).find('.') + 3) + "%";

  std::string pass_rate_color;
  if (pass_rate >= 90.0) {
    pass_rate_color = Colors::GREEN;
  } else if (pass_rate >= 70.0) {
    pass_rate_color = Colors::YELLOW;
  } else {
    pass_rate_color = Colors::RED;
  }

  std::cout << Colors::colorize("Pass Rate: ", Colors::WHITE) << Colors::colorize(pass_rate_str, pass_rate_color + Colors::BOLD) << "\n";
  std::cout << Colors::colorize(separator, Colors::BLUE) << "\n";
}

// Utility functions
bool TestRunner::all_tests_passed() { return get_total_fails() == 0 && get_total_passes() > 0; }

void TestRunner::reset_global_counters() {
  get_total_passes() = 0;
  get_total_fails() = 0;
  get_failed_tests().clear();
}

int TestRunner::get_total_passes_count() { return get_total_passes(); }

int TestRunner::get_total_fails_count() { return get_total_fails(); }

}  // namespace tunit
