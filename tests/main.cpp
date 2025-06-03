#include "tUnit.h"

int main(int argc, char *argv[])
{
  // Run all test suites

  tUnit::Orchestrator::instance().parse_args(argc, argv);
  tUnit::Orchestrator::instance().print_summary();
  tUnit::Orchestrator::instance().write_xml_output();

  return tUnit::Orchestrator::instance().all_tests_passed() ? 0 : 1;
}