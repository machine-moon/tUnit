#include "../../include/orchestrator/test.h"
#include "../../include/orchestrator/orchestrator.h"

namespace tunit {

Test::Test(const std::string &suite_name, const std::string &name)
    : suite_name_(suite_name), name_(name) {}

const std::string &Test::name() const {
  return name_;
}

const std::string &Test::suite_name() const {
  return suite_name_;
}

// Helper function for template implementation
Orchestrator &get_orchestrator_instance() { return Orchestrator::instance(); }

} // namespace tunit
