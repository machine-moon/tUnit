
#include "../../include/test_runner/test_runner_utils.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

#include "../../include/test_runner/colors.h"

namespace fs = std::filesystem;

namespace tunit {

// TestDiscovery Implementation
std::vector<std::string> TestDiscovery::discover_test_executables(const std::string& build_dir) {
  std::vector<std::string> test_executables;

  try {
    for (const auto& entry : fs::directory_iterator(build_dir)) {
      if (entry.is_regular_file() && is_executable_file(entry.path()) && is_test_executable(entry.path().filename().string())) {
        test_executables.push_back(entry.path().string());
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error discovering test executables: " << e.what() << "\n";
  }

  // Sort for consistent ordering
  std::sort(test_executables.begin(), test_executables.end());
  return test_executables;
}

std::string TestDiscovery::find_build_directory() {
  // Check for CMake build directory in common locations
  const std::vector<std::string> build_dirs = {"./build", "../build", "./Build", "../Build"};

  for (const auto& dir : build_dirs) {
    if (fs::exists(dir) && fs::is_directory(dir)) {
      return dir;
    }
  }

  // Fallback to current directory
  return ".";
}

bool TestDiscovery::is_test_executable(const std::string& filename) {
  // Exclude specific runner executables to prevent infinite loops
  const std::vector<std::string> excluded_executables = {"TestRunner", "test_runner", "main"};

  for (const auto& excluded : excluded_executables) {
    if (filename == excluded) {
      return false;
    }
  }

  // Test detection patterns
  const std::vector<std::string> test_patterns = {"Test", "test", "_test", "Tests", "tests", "_tests"};

  for (const auto& pattern : test_patterns) {
    if (filename.find(pattern) != std::string::npos) {
      return true;
    }
  }

  return false;
}

bool TestDiscovery::is_executable_file(const std::filesystem::path& file_path) {
  try {
    auto perms = fs::status(file_path).permissions();
    return (perms & fs::perms::owner_exec) != fs::perms::none;
  } catch (const std::exception&) {
    return false;
  }
}

// TestExecutor Implementation
TestResult TestExecutor::run_test_executable(const std::string& executable_path) {
  TestResult result;
  result.name = fs::path(executable_path).filename().string();
  result.executable_path = executable_path;

  // Generate unique XML filename for this test run
  std::string xml_filename = result.name + "_result.xml";
  std::string xml_path = fs::path(executable_path).parent_path() / xml_filename;

  auto start_time = std::chrono::high_resolution_clock::now();

  // Run executable with XML output enabled
  std::string command = executable_path + " --xml-output=" + xml_path + " 2>&1";
  FILE* pipe = popen(command.c_str(), "r");

  if (pipe == nullptr) {
    result.executable_failed = true;
    result.output = "Failed to execute " + result.name;
    return result;
  }

  std::array<char, 128> buffer{};
  std::ostringstream output_stream;
  while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
    std::string line(buffer.data());
    output_stream << line;
    std::cout << line;
  }

  int exit_code = pclose(pipe);
  auto end_time = std::chrono::high_resolution_clock::now();

  result.execution_time = end_time - start_time;
  result.output = output_stream.str();

  if (exit_code != 0) {
    result.executable_failed = true;
  }

  // Try to read XML results, fallback to stdout parsing if XML not available
  if (fs::exists(xml_path)) {
    parse_xml_results(result, xml_path);
    // Clean up XML file after reading
    std::filesystem::remove(xml_path);
  } else {
    // Fallback to stdout parsing for backward compatibility
    parse_test_output(result, result.output);
  }
  return result;
}

void TestExecutor::parse_test_output(TestResult& result, const std::string& output) {
  std::istringstream iss(output);
  std::string line;
  int pass_count_from_lines = 0;
  int fail_count_from_lines = 0;

  // Parse individual test results
  parse_individual_tests(result, output);

  while (std::getline(iss, line)) {
    if (line.find("[PASS]") != std::string::npos) {
      pass_count_from_lines++;
    } else if (line.find("[FAIL]") != std::string::npos) {
      fail_count_from_lines++;
    }

    // Look for summary line format: "Passed: X, Failed: Y"
    if (line.find("Passed:") != std::string::npos && line.find("Failed:") != std::string::npos) {
      extract_summary_counts(result, line);
    }
  }

  // Use parsed counts if summary wasn't found
  if (result.passed_tests == 0 && result.failed_tests == 0) {
    result.passed_tests = pass_count_from_lines;
    result.failed_tests = fail_count_from_lines;
  }

  result.total_tests = result.passed_tests + result.failed_tests;

  // Handle edge cases
  if (result.total_tests == 0 && !result.executable_failed) {
    result.total_tests = 1;
    result.passed_tests = 1;
    result.failed_tests = 0;
  } else if (result.total_tests == 0 && result.executable_failed) {
    result.total_tests = 1;
    result.passed_tests = 0;
    result.failed_tests = 1;
  }
}

void TestExecutor::parse_individual_tests(TestResult& result, const std::string& output) {
  std::istringstream iss(output);
  std::string line;

  // Regex patterns for parsing test results
  std::regex pass_pattern(R"(\[PASS\]\s+([^:]+):\s*(.+))");
  std::regex fail_pattern(R"(\[FAIL\]\s+([^:]+):\s*(.+))");
  std::smatch match;

  while (std::getline(iss, line)) {
    if (std::regex_search(line, match, pass_pattern)) {
      result.test_names.push_back(match[2].str());
      result.test_results.push_back(true);
    } else if (std::regex_search(line, match, fail_pattern)) {
      result.test_names.push_back(match[2].str());
      result.test_results.push_back(false);
    }
  }
}

void TestExecutor::extract_summary_counts(TestResult& result, const std::string& line) {
  // Extract passed count
  size_t passed_pos = line.find("Passed:");
  if (passed_pos != std::string::npos) {
    size_t start = passed_pos + 7;
    size_t comma = line.find(',', start);
    if (comma != std::string::npos) {
      std::string passed_str = line.substr(start, comma - start);
      passed_str.erase(0, passed_str.find_first_not_of(" \t"));
      passed_str.erase(passed_str.find_last_not_of(" \t") + 1);

      try {
        result.passed_tests = std::stoi(passed_str);
      } catch (const std::exception&) {
        // Keep default value
      }
    }
  }

  // Extract failed count
  size_t failed_pos = line.find("Failed:");
  if (failed_pos != std::string::npos) {
    size_t start = failed_pos + 7;
    std::string failed_str = line.substr(start);
    failed_str.erase(0, failed_str.find_first_not_of(" \t"));
    failed_str.erase(failed_str.find_last_not_of(" \t") + 1);

    try {
      result.failed_tests = std::stoi(failed_str);
    } catch (const std::exception&) {
      // Keep default value
    }
  }
}

// TestReporter Implementation
void TestReporter::print_header(std::string_view title) {
  print_separator();
  std::cout << Colors::colorize("        " + std::string(title), Colors::BLUE + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("        (Enhanced Auto-discovery with tUnit XML)", Colors::BLUE) << "\n";
  print_separator();
  std::cout << "\n";
}

void TestReporter::print_discovery_info(const std::vector<std::string>& test_executables) {
  std::cout << "Discovered " << test_executables.size() << " test executable(s):\n";
  for (const auto& exe : test_executables) {
    std::cout << "  " << Colors::colorize("→", Colors::CYAN) << " " << fs::path(exe).filename().string() << "\n";
  }
  std::cout << "\n";
}

void TestReporter::print_final_summary(const std::vector<TestResult>& results) {
  std::cout << Colors::colorize("--- SUMMARY ---", Colors::BLUE + Colors::BOLD) << "\n";

  for (const auto& result : results) {
    print_test_result_summary(result);
  }
}

void TestReporter::print_test_result_summary(const TestResult& result) {
  if (result.executable_failed) {
    std::cout << Colors::colorize("[EXEC FAIL]", Colors::RED + Colors::BOLD) << " " << Colors::colorize(result.name, Colors::RED) << "\n";
  } else {
    std::string status = (result.failed_tests == 0) ? "[PASS]" : "[FAIL]";
    std::string color = (result.failed_tests == 0) ? Colors::GREEN : Colors::RED;

    std::cout << Colors::colorize(status, color + Colors::BOLD) << " " << Colors::colorize(result.name, Colors::CYAN) << " - " << result.passed_tests << "/" << result.total_tests << " tests passed";

    // Add execution time
    double exec_time = result.execution_time.count();
    std::cout << " (" << std::fixed << std::setprecision(3) << exec_time << "s)";
    std::cout << "\n";
  }
}

void TestReporter::print_stats(const std::vector<TestResult>& results) {
  int total_tests = 0;
  int total_passed = 0;
  int total_failed = 0;
  int failed_executables = 0;
  double total_time = 0.0;

  for (const auto& result : results) {
    total_tests += result.total_tests;
    total_passed += result.passed_tests;
    total_failed += result.failed_tests;
    total_time += result.execution_time.count();

    if (result.executable_failed) {
      failed_executables++;
    }
  }

  std::cout << Colors::colorize("--- STATISTICS ---", Colors::CYAN + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Test Executables Run: ", Colors::WHITE) << Colors::colorize(std::to_string(results.size()), Colors::WHITE + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Total Tests Run: ", Colors::WHITE) << Colors::colorize(std::to_string(total_tests), Colors::WHITE + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Total Passes: ", Colors::GREEN) << Colors::colorize(std::to_string(total_passed), Colors::GREEN + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Total Failures: ", Colors::RED) << Colors::colorize(std::to_string(total_failed), Colors::RED + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Failed Executables: ", Colors::RED) << Colors::colorize(std::to_string(failed_executables), Colors::RED + Colors::BOLD) << "\n";

  // Execution time
  std::cout << Colors::colorize("Total Execution Time: ", Colors::WHITE) << Colors::colorize(std::to_string(total_time).substr(0, 5) + "s", Colors::WHITE + Colors::BOLD) << "\n";

  double success_rate = calculate_success_rate(total_passed, total_tests);
  std::string pass_rate_str = std::to_string(success_rate).substr(0, std::to_string(success_rate).find('.') + 3) + "%";
  std::string pass_rate_color = get_success_rate_color(success_rate);

  std::cout << Colors::colorize("Success Rate: ", Colors::WHITE) << Colors::colorize(pass_rate_str, pass_rate_color + Colors::BOLD) << "\n";

  if (total_failed == 0 && failed_executables == 0) {
    std::cout << Colors::colorize("All tests passed!", Colors::GREEN + Colors::BOLD) << "\n";
  } else {
    std::cout << Colors::colorize("Some tests failed", Colors::RED + Colors::BOLD) << "\n";
  }

  std::cout << Colors::colorize(std::string(60, '-'), Colors::CYAN) << "\n";
}

void TestReporter::generate_tunit_xml(const std::vector<TestResult>& results, const std::string& output_file) {
  auto tUnit_suites = convert_to_tUnit_format(results);
  tUnitXmlWriter::write_xml(tUnit_suites, output_file);

  std::cout << Colors::colorize("tUnit XML report generated: ", Colors::BLUE) << Colors::colorize(output_file, Colors::BLUE + Colors::BOLD) << "\n";
}

std::vector<tUnitTestSuite> TestReporter::convert_to_tUnit_format(const std::vector<TestResult>& results) {
  std::vector<tUnitTestSuite> tUnit_suites;

  for (const auto& result : results) {
    tUnitTestSuite suite;
    suite.name = result.name;
    suite.tests = result.total_tests;
    suite.failures = result.failed_tests;
    suite.errors = result.executable_failed ? 1 : 0;
    suite.time = result.execution_time.count();

    // Convert individual test results
    for (size_t i = 0; i < result.test_names.size(); ++i) {
      tUnitTestCase test_case;
      test_case.name = result.test_names[i];
      test_case.classname = result.name;
      test_case.time = suite.time / suite.tests;  // Approximate time per test
      test_case.passed = result.test_results[i];

      if (!test_case.passed) {
        test_case.failure_message = "Test assertion failed";
        test_case.failure_type = "AssertionError";
      }

      suite.test_cases.push_back(test_case);
    }

    tUnit_suites.push_back(suite);
  }

  return tUnit_suites;
}

int TestReporter::calculate_exit_code(const std::vector<TestResult>& results) {
  for (const auto& result : results) {
    if (result.executable_failed || result.failed_tests > 0) {
      return 1;
    }
  }
  return 0;
}

double TestReporter::calculate_success_rate(int total_passed, int total_tests) { return total_tests > 0 ? (100.0 * total_passed / total_tests) : 0.0; }

std::string TestReporter::get_success_rate_color(double success_rate) {
  if (success_rate >= 90.0) {
    return Colors::GREEN;
  }
  if (success_rate >= 70.0) {
    return Colors::YELLOW;
  }
  return Colors::RED;
}

void TestReporter::print_separator() { std::cout << Colors::colorize(std::string(60, '-'), Colors::BLUE) << "\n"; }

int TestReporter::run(std::string_view title, bool generate_xml, const std::string& xml_output) {
  // Print header
  print_header(title);

  // Discover test executables
  std::string build_dir = TestDiscovery::find_build_directory();
  std::vector<std::string> test_executables = TestDiscovery::discover_test_executables(build_dir);

  if (test_executables.empty()) {
    std::cout << Colors::colorize("No test executables found in ", Colors::RED) << Colors::colorize(build_dir, Colors::RED + Colors::BOLD) << '\n';
    return 1;
  }

  // Print discovery info
  print_discovery_info(test_executables);

  // Run all tests
  std::vector<TestResult> results;
  for (const auto& executable : test_executables) {
    TestResult result = TestExecutor::run_test_executable(executable);
    results.push_back(result);
    std::cout << "\n";
  }

  // Print final summary and stats
  print_final_summary(results);
  print_stats(results);

  // Generate tUnit XML if requested
  if (generate_xml) {
    generate_tunit_xml(results, xml_output);
  }

  return calculate_exit_code(results);
}

}  // namespace tunit
