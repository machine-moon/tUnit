#include "../../include/orchestrator/suite.h"
#include "../../include/orchestrator/orchestrator.h"
#include "../../include/orchestrator/test.h"

namespace tunit {

Suite::Suite(const std::string &name) : name_(name) {}

Test &Suite::get_test(const std::string &test_name) {
  auto it = tests_.find(test_name);
  if (it != tests_.end()) {
    return *(it->second);
  }

  // Create or get the test from orchestrator and store pointer
  Test &test = Orchestrator::instance().get_test(name_, test_name);
  tests_[test_name] = &test;
  return test;
}

const std::string &Suite::name() const {
  return name_;
}

} // namespace tunit
