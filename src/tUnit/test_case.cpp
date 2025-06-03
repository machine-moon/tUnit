#include "tUnit/test_case.h"
#include "tUnit/test_orchestrator.h"

namespace tunit
{

Test::Test(const std::string &suite_name, const std::string &name) : suite_name_(suite_name), name_(name) {}

void Test::expect(const std::string &description, bool condition, bool expected)
{
  bool passed = (condition == expected);

  Assertion assertion(description, passed);

  assertion_ids_.push_back(description);

  Orchestrator::instance().log_assertion(suite_name_, name_, std::move(assertion));
}
const std::string &Test::name() const { return name_; }

const std::string &Test::suite_name() const { return suite_name_; }

// Helper function for template implementation
Orchestrator &get_orchestrator_instance() { return Orchestrator::instance(); }

} // namespace tunit
