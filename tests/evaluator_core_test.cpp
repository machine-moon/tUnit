#include <string>

#include "../include/tunit.h"

namespace pred = tunit::predicates;

int main() {
  auto& suite = tunit::TestRunner::get_suite("Evaluator Core");
  suite.suite_header();

  // Test Evaluator constructor and basic operation
  tunit::Evaluator eval1(10, 20, pred::is_less{});
  suite.test("Evaluator(10, 20, is_less) operator()", eval1());
  suite.test("Evaluator(10, 20, is_less) evaluate()", eval1.evaluate());

  // Test Evaluator use method (new values)
  suite.test("Evaluator use(5, 15)", eval1.use(5, 15));
  suite.test("Evaluator use(25, 15)", eval1.use(25, 15), false);

  // Test Evaluator compare method (new rhs)
  suite.test("Evaluator compare(30)", eval1.compare(30));
  suite.test("Evaluator compare(5)", eval1.compare(5), false);

  // Test with different predicate types
  tunit::Evaluator eval2(5, 5, pred::is_equal{});
  suite.test("Evaluator(5, 5, is_equal)", eval2());

  tunit::Evaluator eval3(10, 5, pred::is_greater{});
  suite.test("Evaluator(10, 5, is_greater)", eval3());

  tunit::Evaluator eval4(3, 7, pred::is_less_equal{});
  suite.test("Evaluator(3, 7, is_less_equal)", eval4());

  // Test with string types
  tunit::Evaluator string_eval(std::string("hello"), std::string("he"), pred::starts_with{});
  suite.test("String Evaluator starts_with", string_eval());
  suite.test("String Evaluator use(world, wo)", string_eval.use(std::string("world"), std::string("wo")));
  suite.test("String Evaluator compare(bye)", string_eval.compare(std::string("bye")), false);

  // Test with different numeric types
  tunit::Evaluator float_eval(3.14, 3.0, pred::is_greater{});
  suite.test("Float Evaluator 3.14 > 3.0", float_eval());

  // Test with custom lambda predicate
  auto custom_pred = [](int a, int b) { return (a + b) > 10; };
  tunit::Evaluator custom_eval(7, 5, custom_pred);
  suite.test("Custom Evaluator (7+5)>10", custom_eval());
  suite.test("Custom Evaluator use(2, 3)", custom_eval.use(2, 3), false);

  // Test evaluator with range check
  auto in_range = [](int val, int max) { return val >= 0 && val <= max; };
  tunit::Evaluator range_eval(5, 10, in_range);
  suite.test("Range Evaluator 5 in [0,10]", range_eval());
  suite.test("Range Evaluator compare(3)", range_eval.compare(3), false);

  // Test negative cases
  tunit::Evaluator fail_eval(20, 10, pred::is_less{});
  suite.test("Evaluator(20, 10, is_less) should fail", fail_eval(), false);

  suite.suite_summary();
  return 0;
}
