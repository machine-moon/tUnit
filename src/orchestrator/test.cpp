#include "../../include/orchestrator/test.h"
#include "../../include/orchestrator/suite.h"

namespace tunit {

Test::Test(const std::string &name)
    : name_(name), assertion_count_(0), failed_assertions_(0),
      assertion_results_() {}

void Test::assert(const std::string &assertion_name, bool condition,
                  bool expected) {
  assertion_count_++;

  bool assertion_passed = (condition == expected); // replaced ASSERT
  if (!assertion_passed) {
    failed_assertions_++;
  }

  // Record result
  if (assertion_results_.find(assertion_name) == assertion_results_.end()) {
    assertion_results_[assertion_name] = assertion_passed;
  } else {
    throw trace::TracedException("Assertion already exists: " + assertion_name);
  }
}

std::size_t Test::get_assertion_count() const { return assertion_count_; }

std::size_t Test::get_failed_assertions_count() const {
  return failed_assertions_;
}

const std::string &Test::get_name() const { return name_; }

bool Test::passed() const {
  return failed_assertions_ == 0 && assertion_count_ > 0;
}

} // namespace tunit
