#include "tUnit/test_orchestrator.h"
#include "tUnit/assertion.h"
#include "tUnit/test_case.h"
#include "tUnit/test_suite.h"
#include "utils/trace_support.h"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace tUnit
{

Orchestrator *Orchestrator::instance_ = nullptr;
Orchestrator::~Orchestrator()
{
  instance_ = nullptr;
}
Orchestrator &Orchestrator::instance()
{
  if (instance_ == nullptr)
  {
    TUNIT_SCOPED_TRACE("creating orchestrator instance");
    instance_ = new Orchestrator();
  }
  return *instance_;
}

Suite &Orchestrator::get_suite(const std::string &name)
{
  auto it = suites_.find(name);
  if (it != suites_.end())
  {
    return *(it->second);
  }

  TUNIT_SCOPED_TRACE("creating suite: " + name);
  suites_[name] = std::make_unique<Suite>(name);
  return *(suites_[name]);
}

Test &Orchestrator::get_test(const std::string &suite_name, const std::string &test_name)
{
  std::string test_key = suite_name + "::" + test_name;

  auto it = tests_.find(test_key);
  if (it != tests_.end())
  {
    return *(it->second);
  }

  TUNIT_SCOPED_TRACE("creating test: " + test_key);
  tests_[test_key] = std::make_unique<Test>(suite_name, test_name);
  return *(tests_[test_key]);
}

void Orchestrator::log_assertion(const std::string &suite_name, const std::string &test_name, Assertion &&assertion)
{
  std::string test_key = suite_name + "::" + test_name;
  assertions_[test_key].emplace_back(std::move(assertion));
}

const std::unordered_map<std::string, std::unique_ptr<Suite>> &Orchestrator::suites() const
{
  return suites_;
}

const std::unordered_map<std::string, std::unique_ptr<Test>> &Orchestrator::tests() const
{
  return tests_;
}

const std::vector<Assertion> &Orchestrator::assertions_for(const std::string &suite_name, const std::string &test_name) const
{
  std::string test_key = suite_name + "::" + test_name;
  auto it = assertions_.find(test_key);
  if (it != assertions_.end())
  {
    return it->second;
  }
  tUnit::trace::throw_traced("No assertions found for test: " + test_key);
}

// High complexity:  O(S * T * A)
bool Orchestrator::all_tests_passed() const
{
  for (const auto &[test_key, assertions] : assertions_)
  {
    for (const auto &assertion : assertions)
    {
      if (!assertion.result_)
      {
        return false;
      }
    }
  }
  return true;
}

size_t Orchestrator::total_assertions() const
{
  size_t total = 0;
  for (const auto &[test_key, assertions] : assertions_)
  {
    total += assertions.size();
  }
  return total;
}

size_t Orchestrator::failed_assertions() const
{
  size_t failed = 0;
  for (const auto &[test_key, assertions] : assertions_)
  {
    for (const auto &assertion : assertions)
    {
      if (!assertion.result_)
      {
        failed++;
      }
    }
  }
  return failed;
}

void Orchestrator::print_summary() const
{
  size_t total = total_assertions();
  size_t failed = failed_assertions();
  size_t passed = total - failed;

  std::cout << "\n";

  // Group tests by suite
  std::unordered_map<std::string, std::vector<std::pair<std::string, const Test *>>> suites_map;
  for (const auto &[test_key, test] : tests_)
  {
    suites_map[test->suite_name()].emplace_back(test->name(), test.get());
  }

  for (const auto &[suite_name, suite_tests] : suites_map)
  {
    std::cout << "--- " << suite_name << " ---\n";

    for (const auto &[test_name, test] : suite_tests)
    {
      std::string test_key = test->suite_name() + "::" + test->name();
      auto assertions_it = assertions_.find(test_key);

      bool test_has_failures = false;
      std::vector<std::string> failed_descriptions;

      if (assertions_it != assertions_.end())
      {
        const auto &assertions = assertions_it->second;
        for (const auto &assertion : assertions)
        {
          if (!assertion.result_)
          {
            test_has_failures = true;
            failed_descriptions.push_back(assertion.description_);
          }
        }
      }

      if (test_has_failures)
      {
        std::cout << "[FAIL] " << test_name << "\n";
        for (const auto &desc : failed_descriptions)
        {
          std::cout << "       " << desc << "\n";
        }
      }
      else
      {
        std::cout << "[PASS] " << test_name << "\n";
      }
    }
    std::cout << "\n";
  }

  std::cout << "--- Total Summary ---\n";
  std::cout << "Total assertions: " << total << "\n";
  std::cout << "Passed: " << passed << "\n";
  std::cout << "Failed: " << failed << "\n";

  if (failed == 0)
  {
    std::cout << "All tests passed!\n";
  }
  else
  {
    std::cout << failed << " assertion(s) failed\n";
  }

  std::cout << "====================\n" << std::endl;
}

void Orchestrator::parse_args(int argc, char *argv[])
{
  for (int i = 1; i < argc; ++i)
  {
    if (std::strcmp(argv[i], "-C") == 0 && i + 1 < argc)
    {
      xml_output_path_ = argv[i + 1];
      ++i;
    }
    else if (std::strcmp(argv[i], "-f") == 0)
    {
      failures_only_ = true;
    }
  }
}

void Orchestrator::write_xml_output() const
{
  // No XML output requested
  if (xml_output_path_.empty())
  {
    return; 
  }

  std::ofstream xml_file(xml_output_path_);
  if (!xml_file.is_open())
  {
    std::cerr << "Error: Could not open XML output file: " << xml_output_path_ << std::endl;
    return;
  }

  // Test statistics
  size_t total_tests = tests_.size();
  size_t failed_tests = 0;
  size_t total_assertions = 0;
  size_t failed_assertions = 0;

  for (const auto &[test_key, test] : tests_)
  {
    auto assertions_it = assertions_.find(test_key);
    if (assertions_it != assertions_.end())
    {
      const auto &assertions = assertions_it->second;
      total_assertions += assertions.size();

      bool test_failed = false;
      for (const auto &assertion : assertions)
      {
        if (!assertion.result_)
        {
          failed_assertions++;
          test_failed = true;
        }
      }
      if (test_failed)
      {
        failed_tests++;
      }
    }
  }

  // XML header
  xml_file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  xml_file << "<testsuites tests=\"" << total_tests << "\" failures=\"" << failed_tests
           << "\" assertions=\"" << total_assertions << "\" failed_assertions=\"" << failed_assertions << "\">\n";

  // Group tests by suite
  std::unordered_map<std::string, std::vector<std::pair<std::string, const Test *>>> suites_map;
  for (const auto &[test_key, test] : tests_)
  {
    suites_map[test->suite_name()].emplace_back(test->name(), test.get());
  }

  // Write each test suite
  for (const auto &[suite_name, suite_tests] : suites_map)
  {
    size_t suite_test_count = suite_tests.size();
    size_t suite_failures = 0;

    // Count failures in this suite
    for (const auto &[test_name, test] : suite_tests)
    {
      std::string test_key = test->suite_name() + "::" + test->name();
      auto assertions_it = assertions_.find(test_key);
      if (assertions_it != assertions_.end())
      {
        const auto &assertions = assertions_it->second;
        for (const auto &assertion : assertions)
        {
          if (!assertion.result_)
          {
            suite_failures++;
            break; // Count as one failed test, not per assertion
          }
        }
      }
    }

    xml_file << "  <testsuite name=\"" << suite_name << "\" tests=\"" << suite_test_count
             << "\" failures=\"" << suite_failures << "\">\n";

    // Write individual test cases
    for (const auto &[test_name, test] : suite_tests)
    {
      std::string test_key = test->suite_name() + "::" + test->name();
      auto assertions_it = assertions_.find(test_key);
      bool test_has_failures = false;

      // Check if test has failures
      if (assertions_it != assertions_.end())
      {
        const auto &assertions = assertions_it->second;
        for (const auto &assertion : assertions)
        {
          if (!assertion.result_)
          {
            test_has_failures = true;
            break;
          }
        }
      }

      // Skip passed tests if failures_only_ is true
      if (failures_only_ && !test_has_failures)
      {
        continue;
      }

      xml_file << "    <testcase name=\"" << test_name << "\" classname=\"" << suite_name << "\">\n";

      // Add failure details if any
      if (test_has_failures && assertions_it != assertions_.end())
      {
        const auto &assertions = assertions_it->second;
        for (const auto &assertion : assertions)
        {
          if (!assertion.result_)
          {
            xml_file << "      <failure message=\"" << assertion.description_ << "\">\n";
            xml_file << "        " << assertion.description_ << "\n";
            xml_file << "      </failure>\n";
          }
        }
      }

      xml_file << "    </testcase>\n";
    }

    xml_file << "  </testsuite>\n";
  }

  xml_file << "</testsuites>\n";
  xml_file.close();

  std::cout << "XML output written to: " << xml_output_path_ << std::endl;
}

} // namespace tUnit
