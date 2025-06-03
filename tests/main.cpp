#include "tUnit.h"

int main()
{
  // Run all test suites

  tUnit::Orchestrator::instance().print_summary();

  return tUnit::Orchestrator::instance().all_tests_passed() ? 0 : 1;
}