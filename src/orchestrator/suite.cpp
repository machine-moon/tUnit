#include "../../include/orchestrator/suite.h"
#include "../../include/orchestrator/orchestrator.h"
#include "../../include/orchestrator/test.h"

namespace tunit {

Suite::Suite(const std::string &name) : name_(name) {}

Test &Suite::get_test(const std::string &test_name) {
  // Check if test name already tracked
  for (const auto &existing_name : test_names_) {
    if (existing_name == test_name) {
      // Test already exists, get it from orchestrator
      return Orchestrator::instance().get_test(name_, test_name);
    }
  }

  // New test, add to our tracking list
  test_names_.push_back(test_name);
  
  // Get/create the test from orchestrator
  return Orchestrator::instance().get_test(name_, test_name);
}

const std::string &Suite::name() const {
  return name_;
}

} // namespace tunit
