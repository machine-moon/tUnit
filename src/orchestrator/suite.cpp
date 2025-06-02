#include "../../include/orchestrator/suite.h"
#include "../../include/orchestrator/orchestrator.h"
#include "../../include/orchestrator/test.h"

namespace tunit {

Suite::Suite(const std::string &name)
    : name_(name), passes_(0), fails_(0), tests_() {}

Test &Suite::get_test(const std::string &test_name) {
  auto it = tests_.find(test_name);
  if (it != tests_.end()) {
    return *(it->second);
  }

  Orchestrator &orchestrator = Orchestrator::get_instance();


  tests_[test_name] = orchestrator.register_test(this, test_name);
  if (tests_[test_name] == nullptr) {
    throw trace::TracedException("Failed to create test: " + test_name);
  }
  // Register the test with the orchestrator

  return *(tests_[test_name]);
}

std::size_t Suite::get_passes_count() const { return passes_; }

std::size_t Suite::get_fails_count() const { return fails_; }

const std::string &Suite::get_name() const { return name_; }

void Suite::record_result(const std::string &test_name, bool passed) {
  if (passed) {
    passes_++;
  } else {
    fails_++;
  }

  // Also record at orchestrator level
  orchestrator_.record_test_result(name_, test_name, passed);
}

} // namespace tunit
