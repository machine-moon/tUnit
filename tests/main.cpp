#include "../include/test_runner/colors.h"
#include "../include/test_runner/test_runner_utils.h"

int main() {
  // Test the enhanced test runner with tUnit XML generation
  return tunit::TestReporter::run("TUnit Enhanced Test Suite", true, "test_results.xml");
}
