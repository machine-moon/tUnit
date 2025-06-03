#include "tunit.h"
#include <stdexcept>
#include <string>

namespace {
namespace pred = tunit::predicates;
using namespace tunit::trace;

auto &suite = tunit::Orchestrator::instance().get_suite("Exception Tracing");

// Helper function to simulate a predicate that throws an exception
bool throwing_predicate(int value) {
  TUNIT_TRACE_FUNCTION();
  if (value < 0) {
    throw_traced("Value must be non-negative, got: " + std::to_string(value));
  }
  return value > 10;
}

// Helper function to simulate nested predicate calls
bool nested_complex_predicate(int value) {
  TUNIT_TRACE_FUNCTION();
  if (value == 42) {
    TUNIT_SCOPED_TRACE("special_case_handler");
    throw_traced("Special case value 42 is not allowed");
  }
  return throwing_predicate(value);
}

// Predicate that uses logical composition with potential exceptions
bool complex_logical_predicate(int value) {
  TUNIT_TRACE_FUNCTION();
  if (value == 999) {
    TUNIT_SCOPED_TRACE("overflow_check");
    throw_traced("Value 999 causes overflow in calculation");
  }
  return nested_complex_predicate(value);
}

void test_basic_exception_tracing() {
  auto &test = suite.get_test("Basic Exception Tracing");
  try {
    throwing_predicate(-5);
    test.expect("Should not reach here after exception", false, true);
  } catch (const TracedException &e) {
    bool has_trace = !e.get_trace_stack().empty();
    test.expect("Basic exception contains trace information", has_trace, true);
    bool correct_message =
        std::string(e.what()).find("Value must be non-negative") !=
        std::string::npos;
    test.expect("Exception message is preserved", correct_message, true);
  } catch (...) {
    test.expect("Unexpected exception type caught", false, true);
  }
}

void test_nested_exception_tracing() {
  auto &test = suite.get_test("Nested Exception Tracing");
  try {
    nested_complex_predicate(42);
    test.expect("Should not reach here after nested exception", false, true);
  } catch (const TracedException &e) {
    auto trace_stack = e.get_trace_stack();
    bool has_nested_traces = trace_stack.size() >= 2;
    test.expect("Nested exception has multiple trace entries",
                has_nested_traces, true);
    bool contains_expected_contexts = false;
    std::string full_trace;
    for (const auto &entry : trace_stack) {
      full_trace += entry.to_string() + " ";
    }
    contains_expected_contexts =
        full_trace.find("nested_complex_predicate") != std::string::npos &&
        full_trace.find("special_case_handler") != std::string::npos;
    test.expect("Trace contains expected predicate contexts",
                contains_expected_contexts, true);
  } catch (...) {
    test.expect("Unexpected exception type in nested test", false, true);
  }
}

void test_deep_nesting_exception_tracing() {
  auto &test = suite.get_test("Deep Nesting Exception Tracing");
  try {
    complex_logical_predicate(999);
    test.expect("Should not reach here after deep nested exception", false,
                true);
  } catch (const TracedException &e) {
    bool has_trace = !e.get_trace_stack().empty();
    test.expect("Deep nested exception contains trace information", has_trace,
                true);
  } catch (...) {
    test.expect("Unexpected exception type in deep nesting test", false, true);
  }
}

struct ExceptionTracingTestRunner {
  ExceptionTracingTestRunner() {
    test_basic_exception_tracing();
    test_nested_exception_tracing();
    test_deep_nesting_exception_tracing();
  }
};

static ExceptionTracingTestRunner runner;

} // anonymous namespace
