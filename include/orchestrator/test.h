#pragma once

#include "../evaluator.h"
#include "../utils/trace_support.h"
#include "orchestrator.h"
#include <cstdlib>
#include <map>
#include <string>

namespace tunit {

class Test {
private:
  std::string name_;
  std::size_t assertion_count_;
  std::size_t failed_assertions_;
  Test(const std::string &name);

public:
  template <typename T, typename P, typename U>
  void assert(const std::string &assertion_name, const T &lhs, P pred,
              const U &rhs, bool expected = true);
  void assert(const std::string &assertion_name, bool condition,
              bool expected = true);

  std::size_t get_assertion_count() const;
  std::size_t get_failed_assertions_count() const;
  const std::string &get_name() const;
  bool passed() const;

  friend class Orchestrator;
};

// Template implementation
template <typename T, typename P, typename U>
void Test::assert(const std::string &assertion_name, const T &lhs, P pred,
                  const U &rhs, bool expected) {
  Evaluator evaluator(lhs, rhs, pred);
  bool result = evaluator();
  assertion_count_++;

  bool assertion_passed = (result == expected); // replaced ASSERT
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

} // namespace tunit
