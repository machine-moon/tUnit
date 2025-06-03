#include "tUnit/test_suite.h"
#include "tUnit/test_case.h"
#include "tUnit/test_orchestrator.h"

namespace tunit
{

Suite::Suite(const std::string &name) : name_(name) {}

Test &Suite::get_test(const std::string &test_name)
{
  auto it = tests_.find(test_name);
  if (it != tests_.end())
  {
    return *(it->second);
  }

  Test &test = Orchestrator::instance().get_test(name_, test_name);
  tests_[test_name] = &test;
  return test;
}

const std::string &Suite::name() const
{
  return name_;
}

} // namespace tunit
