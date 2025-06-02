#pragma once

#include <chrono>
#include <filesystem>
#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "test_runner.h"
#include "tunit_xml.h"

namespace tunit {

struct TestResult {
  std::string name;
  std::string executable_path;
  int total_tests = 0;
  int passed_tests = 0;
  int failed_tests = 0;
  bool executable_failed = false;
  std::string output;
  std::chrono::duration<double> execution_time{};
  std::vector<std::string> test_names;
  std::vector<bool> test_results;
};

// XML parsing functions
void parse_xml_results(TestResult& result, const std::string& xml_path);
void parse_tunit_xml(TestResult& result, const std::string& xml_content);
void extract_attribute(const std::string& tag, const std::string& attr_name, std::function<void(const std::string&)> callback);
void parse_test_cases(TestResult& result, const std::string& xml_content);

class TestDiscovery {
 public:
  static std::vector<std::string> discover_test_executables(const std::string& build_dir);
  static std::string find_build_directory();

 private:
  static bool is_test_executable(const std::string& filename);
  static bool is_executable_file(const std::filesystem::path& file_path);
};

class TestExecutor {
 public:
  static TestResult run_test_executable(const std::string& executable_path);

 private:
  static void parse_test_output(TestResult& result, const std::string& output);
  static void extract_summary_counts(TestResult& result, const std::string& line);
  static void parse_individual_tests(TestResult& result, const std::string& output);
};

class TestReporter {
 public:
  static void print_header(std::string_view title);
  static void print_discovery_info(const std::vector<std::string>& test_executables);
  static void print_final_summary(const std::vector<TestResult>& results);
  static void print_stats(const std::vector<TestResult>& results);
  static int calculate_exit_code(const std::vector<TestResult>& results);
  static void generate_tunit_xml(const std::vector<TestResult>& results, const std::string& output_file);

  // Convenience method to run all tests with a single call
  static int run(std::string_view title, bool generate_xml = false, const std::string& xml_output = "test_results.xml");

 private:
  static void print_separator();
  static void print_test_result_summary(const TestResult& result);
  static double calculate_success_rate(int total_passed, int total_tests);
  static std::string get_success_rate_color(double success_rate);
  static std::vector<tUnitTestSuite> convert_to_tUnit_format(const std::vector<TestResult>& results);
};

}  // namespace tunit
