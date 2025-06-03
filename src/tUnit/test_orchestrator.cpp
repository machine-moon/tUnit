#include "tUnit/test_orchestrator.h"
#include "tUnit/assertion.h"
#include "tUnit/test_case.h"
#include "tUnit/test_suite.h"
#include "utils/trace_support.h"
#include <iostream>

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

  std::cout << "\n=== Test Summary ===" << "\n";
  std::cout << "Total assertions: " << total << "\n";
  std::cout << "Passed: " << passed << "\n";
  std::cout << "Failed: " << failed << "\n";

  if (failed > 0)
  {
    std::cout << "===================" << "\n";
    std::cout << "Failed assertions:" << "\n";
    for (const auto &[test_key, assertions] : assertions_)
    {
      for (const auto &assertion : assertions)
      {
        if (!assertion.result_)
        {
          std::cout << "  [" << test_key << "] " << assertion.description_ << "\n";
        }
      }
    }
  }

  std::cout << "===================" << std::endl;
}

} // namespace tUnit
