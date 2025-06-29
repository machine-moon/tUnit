#pragma once
#include "../evaluator.h"
#include "assertion.h"
#include <sstream>
#include <string>
#include <vector>

namespace tUnit
{

class Assertion;
class Orchestrator;

/**
 * Individual test case that can run assertions and expectations
 */
class Test
{
public:
  Test(const std::string &suite_name, const std::string &name);

  template <typename T, typename P, typename U>
  void assert(const std::string &description, const T &lhs, P pred, const U &rhs);
  void expect(const std::string &description, bool condition, bool expected = true);
  const std::string &name() const;
  const std::string &suite_name() const;

private:
  std::string suite_name_;
  std::string name_;
  std::vector<std::string> assertion_ids_;

  friend class Orchestrator;
};

} // namespace tUnit

#include "tUnit/test_orchestrator.h"

namespace tUnit
{

// Template implementation
template <typename T, typename P, typename U>
void Test::assert(const std::string &description, const T &lhs, P pred, const U &rhs)
{
  Evaluator evaluator(lhs, rhs, pred);
  bool result = evaluator();

  Assertion assertion(description, result);

  assertion_ids_.push_back(description);

  Orchestrator::instance().log_assertion(suite_name_, name_, std::move(assertion));
}

} // namespace tUnit