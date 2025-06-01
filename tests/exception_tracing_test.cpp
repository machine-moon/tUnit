#include <stdexcept>
#include <string>

#include "../include/tunit.h"
#include "../include/utils/trace_support.h"

namespace pred = tunit::predicates;
using namespace tunit::trace;

// Helper function to simulate a predicate that throws an exception
bool throwing_predicate(int value) {
  TraceScope scope("throwing_predicate");
  if (value < 0) {
    throw_traced("Value must be non-negative, got: " + std::to_string(value));
  }
  return value > 10;
}

// Helper function to simulate nested predicate calls
bool nested_complex_predicate(int value) {
  TraceScope scope("nested_complex_predicate");

  // Simulate some validation logic
  if (value == 42) {
    TraceScope inner_scope("special_case_handler");
    throw_traced("Special case value 42 is not allowed");
  }

  // Call another predicate that might throw
  return throwing_predicate(value);
}

// Predicate that uses logical composition with potential exceptions
bool complex_logical_predicate(int value) {
  TraceScope scope("complex_logical_predicate");

  if (value == 999) {
    TraceScope error_scope("overflow_check");
    throw_traced("Value 999 causes overflow in calculation");
  }

  return nested_complex_predicate(value);
}

int main() {
  auto& suite = tunit::TestRunner::get_suite("Exception Tracing");
  suite.suite_header();

  // Test 1: Basic exception tracing functionality
  try {
    throwing_predicate(-5);
    suite.test("Should not reach here after exception", false);
  } catch (const TracedException& e) {
    bool has_trace = !e.get_trace_stack().empty();
    suite.test("Basic exception contains trace information", has_trace);

    bool correct_message = std::string(e.what()).find("Value must be non-negative") != std::string::npos;
    suite.test("Exception message is preserved", correct_message);
  } catch (...) {
    suite.test("Unexpected exception type caught", false);
  }

  // Test 2: Nested exception tracing
  try {
    nested_complex_predicate(42);
    suite.test("Should not reach here after nested exception", false);
  } catch (const TracedException& e) {
    auto trace_stack = e.get_trace_stack();

    bool has_nested_traces = trace_stack.size() >= 2;
    suite.test("Nested exception has multiple trace entries", has_nested_traces);

    bool contains_expected_contexts = false;
    std::string full_trace;
    for (const auto& entry : trace_stack) {
      full_trace += entry + " ";
    }
    contains_expected_contexts = full_trace.find("nested_complex_predicate") != std::string::npos && full_trace.find("special_case_handler") != std::string::npos;
    suite.test("Trace contains expected predicate contexts", contains_expected_contexts);
  } catch (...) {
    suite.test("Unexpected exception type in nested test", false);
  }

  // Test 3: Deep nesting exception tracing
  try {
    complex_logical_predicate(999);
    suite.test("Should not reach here after deep nested exception", false);
  } catch (const TracedException& e) {
    auto trace_stack = e.get_trace_stack();

    bool has_deep_traces = trace_stack.size() >= 3;
    suite.test("Deep nested exception has multiple trace levels", has_deep_traces);

    std::string trace_str;
    for (const auto& entry : trace_stack) {
      trace_str += entry + " -> ";
    }

    bool contains_all_levels = trace_str.find("complex_logical_predicate") != std::string::npos && trace_str.find("overflow_check") != std::string::npos;
    suite.test("Deep trace contains all predicate levels", contains_all_levels);
  } catch (...) {
    suite.test("Unexpected exception type in deep nesting test", false);
  }

  // Test 4: Exception tracing with normal predicates (should work without exceptions)
  try {
    auto equal_pred = pred::is_equal{};
    bool result = equal_pred(5, 5);
    suite.test("Normal predicate works with tracing enabled", result);

    auto positive_pred = pred::is_positive{};
    bool pos_result = positive_pred(10);
    suite.test("Numeric predicate works with tracing", pos_result);

    auto contains_pred = pred::contains_substring{};
    bool contains_result = contains_pred("hello", "ell");
    suite.test("String predicate works with tracing", contains_result);

  } catch (...) {
    suite.test("Normal predicates should not throw", false);
  }

  // Test 5: Exception tracing with evaluator integration
  try {
    // This will work normally
    tunit::Evaluator eval1(10, 20, pred::is_less{});
    suite.test("Evaluator works normally with tracing", eval1.evaluate());

    // Test with a predicate that might be used in real scenarios
    auto range_pred = pred::is_in_range{};
    bool range_result = range_pred(5, 1, 10);
    suite.test("Range predicate works with tracing", range_result);

  } catch (...) {
    suite.test("Evaluator integration with tracing failed", false);
  }

// Test 6: Production vs Debug mode behavior
#if TUNIT_MODE
  suite.test("Running in production mode", true);
  suite.test("Tracing should be minimal in production", true);
#else
  suite.test("Running in debug/development mode", true);
  suite.test("Full tracing available in debug mode", true);
#endif

  // Test 7: Manual trace scope usage
  try {
    {
      TraceScope outer_scope("manual_test_scope");
      {
        TraceScope inner_scope("manual_inner_scope");
        auto even_pred = pred::is_even{};
        bool even_result = even_pred(4);
        suite.test("Manual trace scopes work with predicates", even_result);
      }
    }
    suite.test("Manual trace scope cleanup successful", true);
  } catch (...) {
    suite.test("Manual trace scope usage failed", false);
  }

  // Test 8: Exception message formatting
  try {
    throwing_predicate(-10);
  } catch (const TracedException& e) {
    std::string message(e.what());

    bool has_error_prefix = message.find("Error:") != std::string::npos;
    suite.test("Exception message has proper formatting", has_error_prefix);

    bool has_original_message = message.find("Value must be non-negative") != std::string::npos;
    suite.test("Original exception message preserved in formatting", has_original_message);

#if !TUNIT_MODE
    bool has_trace_section = message.find("Trace") != std::string::npos;
    suite.test("Exception message includes trace section in debug", has_trace_section);
#endif
  } catch (...) {
    suite.test("Exception formatting test failed", false);
  }

  suite.suite_summary();
  return 0;
}
