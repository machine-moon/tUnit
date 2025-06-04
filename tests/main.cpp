#include "tUnit.h"

int main(int argc, char *argv[])
{
  // All test suites self register
  tUnit::Orchestrator::instance().parse_args(argc, argv);
  tUnit::Orchestrator::instance().write_xml_output();
  tUnit::Orchestrator::instance().print_summary();
  return tUnit::Orchestrator::instance().all_tests_passed() ? 0 : 1;
}