#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "../evaluator.h"
#include "assertion.h"

namespace tunit {

class Assertion;
class Orchestrator;

class Test {
public:
  Test(const std::string &suite_name, const std::string &name);

  template <typename L, typename Predicate, typename R>
  void assert(const std::string &description, L &&lhs, Predicate pred, R &&rhs);

  const std::string &name() const;
  const std::string &suite_name() const;

private:
  std::string suite_name_;
  std::string name_;
  std::vector<std::string> assertion_ids_;

  friend class Orchestrator;
};

} // namespace tunit

// Include this after the class declaration to avoid circular dependencies
#include "orchestrator.h"

namespace tunit {

// Template implementation
template <typename L, typename Predicate, typename R>
void Test::assert(const std::string &description, L &&lhs, Predicate pred, R &&rhs) {
  // Create evaluator to test the assertion
  Evaluator evaluator(lhs, rhs, pred);
  bool passed = evaluator();
  
  // Convert values to strings for logging
  std::ostringstream lhs_stream, rhs_stream;
  lhs_stream << lhs;
  rhs_stream << rhs;
  
  // Create assertion object
  Assertion assertion(description, passed, lhs_stream.str(), rhs_stream.str());
  
  // Store assertion ID for tracking
  assertion_ids_.push_back(description);

  // Log with orchestrator
  Orchestrator::instance().log_assertion(suite_name_, name_,
                                         std::move(assertion));
}

} // namespace tunit