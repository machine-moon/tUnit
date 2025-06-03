#include "tunit.h"
#include <string>

namespace
{
namespace pred = tunit::predicates;

auto &suite = tunit::Orchestrator::instance().get_suite("Evaluator Core");

void test_basic_evaluator_operations()
{
  auto &test = suite.get_test("Basic Evaluator Operations");
  tunit::Evaluator eval1(10, 20, pred::is_less{});
  test.expect("Evaluator(10, 20, is_less) operator()", eval1(), true);
  test.expect("Evaluator(10, 20, is_less) evaluate()", eval1.evaluate(), true);
}

void test_evaluator_use_method()
{
  auto &test = suite.get_test("Evaluator Use Method");
  tunit::Evaluator eval1(10, 20, pred::is_less{});
  test.expect("Evaluator use(5, 15)", eval1.use(5, 15), true);
  test.expect("Evaluator use(25, 15)", eval1.use(25, 15), false);
}

void test_evaluator_compare_method()
{
  auto &test = suite.get_test("Evaluator Compare Method");
  tunit::Evaluator eval1(10, 20, pred::is_less{});
  test.expect("Evaluator compare(30)", eval1.compare(30), true);
  test.expect("Evaluator compare(5)", eval1.compare(5), false);
}

void test_different_predicate_types()
{
  auto &test = suite.get_test("Different Predicate Types");
  tunit::Evaluator eval2(5, 5, pred::is_equal{});
  test.expect("Evaluator(5, 5, is_equal)", eval2(), true);
  tunit::Evaluator eval3(10, 5, pred::is_greater{});
  test.expect("Evaluator(10, 5, is_greater)", eval3(), true);
  tunit::Evaluator eval4(3, 7, pred::is_less_equal{});
  test.expect("Evaluator(3, 7, is_less_equal)", eval4(), true);
}

void test_string_evaluators()
{
  auto &test = suite.get_test("String Evaluators");
  tunit::Evaluator string_eval(std::string("hello"), std::string("he"), pred::starts_with{});
  test.expect("String Evaluator starts_with", string_eval(), true);
  test.expect("String Evaluator use(world, wo)", string_eval.use(std::string("world"), std::string("wo")), true);
  test.expect("String Evaluator compare(bye)", string_eval.compare(std::string("bye")), false);
}

void test_numeric_and_custom_evaluators()
{
  auto &test = suite.get_test("Numeric and Custom Evaluators");
  tunit::Evaluator float_eval(3.14, 3.0, pred::is_greater{});
  test.expect("Float Evaluator 3.14 > 3.0", float_eval(), true);
  auto custom_pred = [](int a, int b)
  { return (a + b) > 10; };
  tunit::Evaluator custom_eval(7, 5, custom_pred);
  test.expect("Custom Evaluator (7+5)>10", custom_eval(), true);
  test.expect("Custom Evaluator use(2, 3)", custom_eval.use(2, 3), false);
  auto in_range = [](int val, int max)
  { return val >= 0 && val <= max; };
  tunit::Evaluator range_eval(5, 10, in_range);
  test.expect("Range Evaluator 5 in [0,10]", range_eval(), true);
  test.expect("Range Evaluator compare(3)", range_eval.compare(3), false);
}

void test_negative_cases()
{
  auto &test = suite.get_test("Negative Cases");
  tunit::Evaluator fail_eval(20, 10, pred::is_less{});
  test.expect("Evaluator(20, 10, is_less) should fail", fail_eval(), false);
}

struct TestRunner
{
  TestRunner()
  {
    test_basic_evaluator_operations();
    test_evaluator_use_method();
    test_evaluator_compare_method();
    test_different_predicate_types();
    test_string_evaluators();
    test_numeric_and_custom_evaluators();
    test_negative_cases();
  }
};

static TestRunner runner;

} // anonymous namespace
