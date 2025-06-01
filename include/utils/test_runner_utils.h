#pragma once

#include <filesystem>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "test_runner.h"

namespace tunit {

struct TestResult {
  std::string name;
  int total_tests = 0;
  int passed_tests = 0;
  int failed_tests = 0;
  bool executable_failed = false;
  std::string output;
};

class TestDiscovery {
 public:
  static std::vector<std::string> discover_test_executables(const std::string& build_dir);
  static std::string find_build_directory();
};

class TestExecutor {
 public:
  static TestResult run_test_executable(const std::string& executable_path);

 private:
  static void parse_test_output(TestResult& result, const std::string& output);
  static void extract_summary_counts(TestResult& result, const std::string& line);
};

class TestReporter {
 public:
  static void print_header(std::string_view title);
  static void print_discovery_info(const std::vector<std::string>& test_executables);
  static void print_final_summary(const std::vector<TestResult>& results);
  static void print_stats(const std::vector<TestResult>& results);
  static int calculate_exit_code(const std::vector<TestResult>& results);

  // Convenience method to run all tests with a single call
  static int run(std::string_view title);

 private:
  static void print_separator();
  static void print_test_result_summary(const TestResult& result);
  static double calculate_success_rate(int total_passed, int total_tests);
  static std::string get_success_rate_color(double success_rate);
};

}  // namespace tunit
