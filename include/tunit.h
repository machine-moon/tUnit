#pragma once

// Single header
#include "evaluator.h"
#include "predicates/all_predicates.h"
#include "test_runner/test_runner.h"
#include "test_runner/test_runner_utils.h"

// This header provides all core components of the tunit library:
// - Core evaluator functionality from evaluator.h
// - All predicates from predicates/all_predicates.h
// - Test runner framework (test discovery, execution, reporting) from test_runner.h and test_runner_utils.h

// Convenience macro for XML-enabled test suites
#define TUNIT_MAIN()                                             \
  void run_tests();                                              \
  int main(int argc, char* argv[]) {                             \
    std::string xml_output_path;                                 \
    for (int i = 1; i < argc; ++i) {                             \
      std::string arg(argv[i]);                                  \
      if (arg.find("--xml-output=") == 0) {                      \
        xml_output_path = arg.substr(13);                        \
        tunit::TestRunner::set_xml_output_enabled(true);         \
        tunit::TestRunner::set_xml_output_path(xml_output_path); \
        break;                                                   \
      }                                                          \
    }                                                            \
    run_tests();                                                 \
    tunit::TestRunner::global_summary();                         \
    if (!xml_output_path.empty()) {                              \
      tunit::TestRunner::generate_tunit_xml();                   \
    }                                                            \
    return tunit::TestRunner::all_tests_passed() ? 0 : 1;        \
  }                                                              \
  void run_tests()
