#include "../../include/test_runner/test_runner_utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

namespace tunit {

void parse_xml_results(TestResult& result, const std::string& xml_path) {
  try {
    std::ifstream xml_file(xml_path);
    if (!xml_file.is_open()) {
      std::cerr << "Warning: Could not open XML file: " << xml_path << std::endl;
      return;
    }

    std::string xml_content((std::istreambuf_iterator<char>(xml_file)),
                            std::istreambuf_iterator<char>());
    xml_file.close();

    // Parse XML content to extract test results
    parse_tunit_xml(result, xml_content);
  } catch (const std::exception& e) {
    std::cerr << "Error parsing XML results: " << e.what() << std::endl;
  }
}

void parse_tunit_xml(TestResult& result, const std::string& xml_content) {
  // Simple string-based XML parsing for tUnit format

  // Find testsuite element
  size_t testsuite_start = xml_content.find("<testsuite");
  if (testsuite_start == std::string::npos) {
    return;
  }
  
  size_t testsuite_end = xml_content.find(">", testsuite_start);
  if (testsuite_end == std::string::npos) {
    return;
  }
  
  std::string testsuite_tag = xml_content.substr(testsuite_start, testsuite_end - testsuite_start);
  
  // Extract attributes using simple string parsing
  extract_attribute(testsuite_tag, "tests", [&result](const std::string& value) {
    try {
      result.total_tests = std::stoi(value);
    } catch (...) {}
  });
  
  int failures = 0, errors = 0;
  extract_attribute(testsuite_tag, "failures", [&failures](const std::string& value) {
    try {
      failures = std::stoi(value);
    } catch (...) {}
  });
  
  extract_attribute(testsuite_tag, "errors", [&errors](const std::string& value) {
    try {
      errors = std::stoi(value);
    } catch (...) {}
  });
  
  result.failed_tests = failures + errors;
  result.passed_tests = result.total_tests - result.failed_tests;
  
  extract_attribute(testsuite_tag, "time", [&result](const std::string& value) {
    try {
      double time_seconds = std::stod(value);
      result.execution_time = std::chrono::duration<double>(time_seconds);
    } catch (...) {}
  });
  
  // Parse individual test cases
  parse_test_cases(result, xml_content);
}

void extract_attribute(const std::string& tag, const std::string& attr_name, 
                      std::function<void(const std::string&)> callback) {
  std::string search_pattern = attr_name + "=\"";
  size_t attr_start = tag.find(search_pattern);
  if (attr_start == std::string::npos) {
    return;
  }
  
  attr_start += search_pattern.length();
  size_t attr_end = tag.find("\"", attr_start);
  if (attr_end == std::string::npos) {
    return;
  }
  
  std::string attr_value = tag.substr(attr_start, attr_end - attr_start);
  callback(attr_value);
}

void parse_test_cases(TestResult& result, const std::string& xml_content) {
  size_t pos = 0;
  std::string testcase_start = "<testcase";
  std::string testcase_end = "</testcase>";
  
  while ((pos = xml_content.find(testcase_start, pos)) != std::string::npos) {
    size_t case_start = pos;
    size_t case_end = xml_content.find(testcase_end, pos);
    if (case_end == std::string::npos) {
      break;
    }
    case_end += testcase_end.length();
    
    std::string testcase_content = xml_content.substr(case_start, case_end - case_start);
    
    // Extract test name
    std::string test_name;
    extract_attribute(testcase_content, "name", [&test_name](const std::string& value) {
      test_name = value;
    });
    
    if (!test_name.empty()) {
      result.test_names.push_back(test_name);
      
      // Check for failures or errors
      bool has_failure = testcase_content.find("<failure") != std::string::npos;
      bool has_error = testcase_content.find("<error") != std::string::npos;
      
      result.test_results.push_back(!(has_failure || has_error));
    }
    
    pos = case_end;
  }
}

} // namespace tunit
