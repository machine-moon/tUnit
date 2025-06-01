#include "../include/utils/test_runner_utils.h"
#include "../include/utils/colors.h"

int main() {
  // Test the enhanced test runner with JUnit XML generation
  return tunit::TestReporter::run("TUnit Enhanced Test Suite", true, "test_results.xml");
}
