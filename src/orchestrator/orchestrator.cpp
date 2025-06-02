#include "../../include/orchestrator/orchestrator.h"
#include "../../include/orchestrator/suite.h"
#include "../../include/orchestrator/test.h"

#include <stdexcept>

namespace tunit {

// Static member definition
std::unique_ptr<Orchestrator> Orchestrator::instance_ = nullptr;

Orchestrator::Orchestrator()
    : total_passes_(0), total_fails_(0), suites_(), tests_(),
      assertion_results_() {}

Orchestrator &Orchestrator::get_instance() {
  if (!instance_) {
    instance_ = std::unique_ptr<Orchestrator>(new Orchestrator());
  }
  return *instance_;
}

Suite &Orchestrator::register_suite(const std::string &suite_name) {
  auto it = suites_.find(suite_name);
  if (it != suites_.end()) {
    return *(it->second);
  }

  Suite *suite = new Suite(suite_name);
  suites_[suite_name] = suite;
  return *suite;
}
Test &Orchestrator::register_test(Suite *suite, const std::string &test_name) {
  auto suite_it = suites_.find(suite->get_name());
  if (suite_it == suites_.end() || suite_it->second != suite) {
    throw std::runtime_error("Suite not registered for test: " + test_name);
  }

  auto &suite_tests = tests_[suite];

  auto test_it = suite_tests.find(test_name);
  if (test_it != suite_tests.end()) {
    throw std::runtime_error("Test already registered: " + test_name);
  }

  Test *test = new Test(test_name);
  suite_tests[test_name] = test;
  return *test;
}

void Orchestrator::register_result(Test *test, bool result) {
  assertion_results_[test] = result;
  if (result) {
    total_passes_++;
  } else {
    total_fails_++;
  }
}

Test &Orchestrator::get_test(const std::string &test_name) {
  for (auto &suite_pair : tests_) {
    auto &suite_tests = suite_pair.second;
    auto test_it = suite_tests.find(test_name);
    if (test_it != suite_tests.end()) {
      return *(test_it->second);
    }
  }
  throw std::runtime_error("Test not found: " + test_name);
}

Suite &Orchestrator::get_suite(const std::string &suite_name) {
  auto it = suites_.find(suite_name);
  if (it == suites_.end()) {
    throw std::runtime_error("Suite not found: " + suite_name);
  }
  return *(it->second);
}

bool Orchestrator::all_suites_passed() const {
  return total_fails_ == 0 && total_passes_ > 0;
}

std::size_t Orchestrator::get_total_passes_count() const {
  return total_passes_;
}

std::size_t Orchestrator::get_total_fails_count() const { return total_fails_; }

void Orchestrator::record_test_result(const std::string &file_name) {
  // Implementation depends on your requirements.
  // For now, this can be left empty or log the file name.
}

} // namespace tunit
