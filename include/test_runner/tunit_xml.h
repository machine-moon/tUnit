#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace tunit {

struct tUnitTestCase {
  std::string name;
  std::string classname;
  double time = 0.0;
  bool passed = true;
  std::string failure_message;
  std::string failure_type;
};

struct tUnitTestSuite {
  std::string name;
  int tests = 0;
  int failures = 0;
  int errors = 0;
  double time = 0.0;
  std::vector<tUnitTestCase> test_cases;
};

class tUnitXmlWriter {
 public:
  static void write_xml(const std::vector<tUnitTestSuite>& test_suites, const std::string& output_file);
  static std::string generate_xml(const std::vector<tUnitTestSuite>& test_suites);

 private:
  static std::string escape_xml(const std::string& input);
  static std::string format_time(double seconds);
};

}  // namespace tunit
