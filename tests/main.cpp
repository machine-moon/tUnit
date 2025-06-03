#include "tunit.h"

int main()
{
  // Run all test suites

  tunit::Orchestrator::instance().print_summary();

  return tunit::Orchestrator::instance().all_tests_passed() ? 0 : 1;
}