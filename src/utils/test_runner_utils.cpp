#include "../../include/utils/test_runner_utils.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;

namespace tunit {

std::vector<std::string> TestDiscovery::discover_test_executables(const std::string& build_dir) {
  std::vector<std::string> test_executables;

  try {
    for (const auto& entry : fs::directory_iterator(build_dir)) {
      if (entry.is_regular_file()) {
        std::string filename = entry.path().filename().string();

        if (filename == "TestRunner" || filename.find("cmake") != std::string::npos || filename.find("Makefile") != std::string::npos || filename.find('.') != std::string::npos) {
          continue;
        }

        if ((filename.find("Test") != std::string::npos) || (filename.find("test") != std::string::npos)) {
          test_executables.push_back(entry.path().string());
        }
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error discovering test executables: " << e.what() << "\n";
  }

  return test_executables;
}

std::string TestDiscovery::find_build_directory() {
  // Check for CMake build directory
  std::string build_dir = "./build";
  if (fs::exists(build_dir)) {
    return build_dir;
  }

  build_dir = "../build";
  if (fs::exists(build_dir)) {
    return build_dir;
  }

  return ".";
}

TestResult TestExecutor::run_test_executable(const std::string& executable_path) {
  TestResult result;
  result.name = fs::path(executable_path).filename().string();

  std::string command = executable_path + " 2>&1";
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
  result.output = output_stream.str();

  if (exit_code != 0) {
    result.executable_failed = true;
  }

  parse_test_output(result, result.output);
  return result;
}

void TestExecutor::parse_test_output(TestResult& result, const std::string& output) {
  std::istringstream iss(output);
  std::string line;
  int pass_count_from_lines = 0;
  int fail_count_from_lines = 0;

  while (std::getline(iss, line)) {
    if (line.find("[PASS]") != std::string::npos) {
      pass_count_from_lines++;
    } else if (line.find("[FAIL]") != std::string::npos) {
      fail_count_from_lines++;
    }

    if (line.find("Passed:") != std::string::npos && line.find("Failed:") != std::string::npos) {
      extract_summary_counts(result, line);
    }
  }

  if (result.passed_tests == 0 && result.failed_tests == 0) {
    result.passed_tests = pass_count_from_lines;
    result.failed_tests = fail_count_from_lines;
  }

  result.total_tests = result.passed_tests + result.failed_tests;

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

void TestExecutor::extract_summary_counts(TestResult& result, const std::string& line) {
  size_t passed_pos = line.find("Passed:");
  if (passed_pos != std::string::npos) {
    size_t start = passed_pos + 7;
    size_t comma = line.find(',', start);
    if (comma != std::string::npos) {
      std::string passed_str = line.substr(start, comma - start);
      passed_str.erase(0, passed_str.find_first_not_of(" \t"));
      passed_str.erase(passed_str.find_last_not_of(" \t") + 1);

      try {
        if (!passed_str.empty()) {
          result.passed_tests = std::stoi(passed_str);
        }
      } catch (const std::exception& ex) {
        std::cerr << "Error parsing passed_tests from '" << passed_str << "': " << ex.what() << "\n";
        // Don't throw - just continue
      }
    }
  }

  size_t failed_pos = line.find("Failed:");
  if (failed_pos != std::string::npos) {
    size_t start = failed_pos + 7;
    std::string failed_str = line.substr(start);
    failed_str.erase(0, failed_str.find_first_not_of(" \t"));
    failed_str.erase(failed_str.find_last_not_of(" \t") + 1);

    try {
      if (!failed_str.empty()) {
        result.failed_tests = std::stoi(failed_str);
      }
    } catch (const std::exception& ex) {
      std::cerr << "Error parsing failed_tests from '" << failed_str << "': " << ex.what() << "\n";
      // Don't throw - just continue
    }
  }
}

void TestReporter::print_header(std::string_view title) {
  print_separator();
  std::cout << Colors::colorize("        " + std::string(title), Colors::BLUE + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("        (Auto-discovering test executables)", Colors::BLUE) << "\n";
  print_separator();
  std::cout << "\n";
}

void TestReporter::print_discovery_info(const std::vector<std::string>& test_executables) {
  std::cout << "Discovered " << test_executables.size() << " test executable(s):\n";
  for (const auto& exe : test_executables) {
    std::cout << "  - " << fs::path(exe).filename().string() << "\n";
  }
  std::cout << "\n";
}

void TestReporter::print_final_summary(const std::vector<TestResult>& results) {
  std::cout << Colors::colorize("--- SUMMARY ---", Colors::BLUE + Colors::BOLD) << "\n";

  for (const auto& result : results) {
    print_test_result_summary(result);
  }
}

void TestReporter::print_stats(const std::vector<TestResult>& results) {
  int total_tests = 0;
  int total_passed = 0;
  int total_failed = 0;
  int failed_executables = 0;

  for (const auto& result : results) {
    total_tests += result.total_tests;
    total_passed += result.passed_tests;
    total_failed += result.failed_tests;
    if (result.executable_failed) {
      failed_executables++;
    }
  }

  std::cout << Colors::colorize("--- Stats ---", Colors::CYAN + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Test Executables Run: ", Colors::WHITE) << Colors::colorize(std::to_string(results.size()), Colors::WHITE + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Total Tests Run: ", Colors::WHITE) << Colors::colorize(std::to_string(total_tests), Colors::WHITE + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Total Passes: ", Colors::GREEN) << Colors::colorize(std::to_string(total_passed), Colors::GREEN + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Total Failures: ", Colors::RED) << Colors::colorize(std::to_string(total_failed), Colors::RED + Colors::BOLD) << "\n";
  std::cout << Colors::colorize("Failed Executables: ", Colors::RED) << Colors::colorize(std::to_string(failed_executables), Colors::RED + Colors::BOLD) << "\n";

  double success_rate = calculate_success_rate(total_passed, total_tests);
  std::string pass_rate_str = std::to_string(success_rate).substr(0, std::to_string(success_rate).find('.') + 3) + "%";
  std::string pass_rate_color = get_success_rate_color(success_rate);

  std::cout << Colors::colorize("Success Rate: ", Colors::WHITE) << Colors::colorize(pass_rate_str, pass_rate_color + Colors::BOLD) << "\n";

  if (total_failed == 0 && failed_executables == 0) {
    std::cout << Colors::colorize("ALL TESTS PASSED!", Colors::GREEN + Colors::BOLD) << "\n";
  } else {
    std::cout << Colors::colorize("Some tests failed. Details above.", Colors::RED) << "\n";
  }

  std::cout << Colors::colorize(std::string(60, '-'), Colors::CYAN) << "\n";
}

int TestReporter::calculate_exit_code(const std::vector<TestResult>& results) {
  for (const auto& result : results) {
    if (result.failed_tests > 0 || result.executable_failed) {
      return 1;
    }
  }
  return 0;
}

void TestReporter::print_separator() { std::cout << Colors::colorize(std::string(60, '-'), Colors::BLUE) << "\n"; }

void TestReporter::print_test_result_summary(const TestResult& result) {
  if (result.executable_failed) {
    std::cout << Colors::colorize("[FAIL]", Colors::RED + Colors::BOLD) << " " << Colors::colorize(result.name, Colors::CYAN) << " (FAILED TO RUN)\n";
  } else if (result.failed_tests == 0) {
    std::cout << Colors::colorize("[PASS]", Colors::GREEN + Colors::BOLD) << " " << Colors::colorize(result.name, Colors::CYAN) << " (" << Colors::colorize(std::to_string(result.passed_tests), Colors::GREEN) << "/" << result.total_tests << " passed)\n";
  } else {
    std::cout << Colors::colorize("[PARTIAL]", Colors::YELLOW + Colors::BOLD) << " " << Colors::colorize(result.name, Colors::CYAN) << " (" << Colors::colorize(std::to_string(result.passed_tests), Colors::GREEN) << "/" << result.total_tests << " passed)\n";
  }
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

int TestReporter::run(std::string_view title) {
  // Print header
  print_header(title);

  // Discover test executables
  std::string build_dir = TestDiscovery::find_build_directory();
  std::vector<std::string> test_executables = TestDiscovery::discover_test_executables(build_dir);

  if (test_executables.empty()) {
    std::cout << "No test executables found in " << build_dir << '\n';
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

  return calculate_exit_code(results);
}

}  // namespace tunit
