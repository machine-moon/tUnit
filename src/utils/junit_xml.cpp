#include "../../include/utils/junit_xml.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace tunit {

void JUnitXmlWriter::write_xml(const std::vector<JUnitTestSuite>& test_suites, const std::string& output_file) {
  std::ofstream file(output_file);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file for writing: " + output_file);
  }
  
  file << generate_xml(test_suites);
  file.close();
}

std::string JUnitXmlWriter::generate_xml(const std::vector<JUnitTestSuite>& test_suites) {
  std::ostringstream xml;
  
  // XML header
  xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  
  // Calculate totals
  int total_tests = 0;
  int total_failures = 0;
  int total_errors = 0;
  double total_time = 0.0;
  
  for (const auto& suite : test_suites) {
    total_tests += suite.tests;
    total_failures += suite.failures;
    total_errors += suite.errors;
    total_time += suite.time;
  }
  
  // Root testsuites element
  xml << "<testsuites name=\"TUnit Test Results\" tests=\"" << total_tests 
      << "\" failures=\"" << total_failures 
      << "\" errors=\"" << total_errors 
      << "\" time=\"" << format_time(total_time) << "\">\n";
  
  // Individual test suites
  for (const auto& suite : test_suites) {
    xml << "  <testsuite name=\"" << escape_xml(suite.name) 
        << "\" tests=\"" << suite.tests 
        << "\" failures=\"" << suite.failures 
        << "\" errors=\"" << suite.errors 
        << "\" time=\"" << format_time(suite.time) << "\">\n";
    
    // Test cases
    for (const auto& test_case : suite.test_cases) {
      xml << "    <testcase name=\"" << escape_xml(test_case.name) 
          << "\" classname=\"" << escape_xml(test_case.classname) 
          << "\" time=\"" << format_time(test_case.time) << "\"";
      
      if (test_case.passed) {
        xml << "/>\n";
      } else {
        xml << ">\n";
        xml << "      <failure message=\"" << escape_xml(test_case.failure_message) 
            << "\" type=\"" << escape_xml(test_case.failure_type) << "\"/>\n";
        xml << "    </testcase>\n";
      }
    }
    
    xml << "  </testsuite>\n";
  }
  
  xml << "</testsuites>\n";
  return xml.str();
}

std::string JUnitXmlWriter::escape_xml(const std::string& input) {
  std::string result = input;
  std::replace(result.begin(), result.end(), '&', '&');
  
  size_t pos = 0;
  while ((pos = result.find("&", pos)) != std::string::npos) {
    if (result.substr(pos, 5) != "&amp;" && 
        result.substr(pos, 4) != "&lt;" && 
        result.substr(pos, 4) != "&gt;" && 
        result.substr(pos, 6) != "&quot;" && 
        result.substr(pos, 6) != "&apos;") {
      result.replace(pos, 1, "&amp;");
      pos += 5;
    } else {
      pos++;
    }
  }
  
  pos = 0;
  while ((pos = result.find("<", pos)) != std::string::npos) {
    result.replace(pos, 1, "&lt;");
    pos += 4;
  }
  
  pos = 0;
  while ((pos = result.find(">", pos)) != std::string::npos) {
    result.replace(pos, 1, "&gt;");
    pos += 4;
  }
  
  pos = 0;
  while ((pos = result.find("\"", pos)) != std::string::npos) {
    result.replace(pos, 1, "&quot;");
    pos += 6;
  }
  
  return result;
}

std::string JUnitXmlWriter::format_time(double seconds) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(3) << seconds;
  return ss.str();
}

}  // namespace tunit
