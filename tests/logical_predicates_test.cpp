#include "tunit.h"

namespace {
namespace pred = tunit::predicates;

auto &suite = tunit::Orchestrator::instance().get_suite("Logical Predicates");

void test_and_predicate() {
  auto& test = suite.get_test("and_predicate");
  test.expect("4 satisfies and_(positive, even)", pred::and_{pred::is_positive{}, pred::is_even{}}(4), true);
  test.expect("3 fails and_(positive, even)", pred::and_{pred::is_positive{}, pred::is_even{}}(3), false);
  test.expect("-4 fails and_(positive, even)", pred::and_{pred::is_positive{}, pred::is_even{}}(-4), false);
  test.expect("-3 fails and_(positive, even)", pred::and_{pred::is_positive{}, pred::is_even{}}(-3), false);
}

void test_or_predicate() {
  auto &test = suite.get_test("or_predicate");
  test.expect("0 satisfies or_(zero, positive)", pred::or_{pred::is_zero{}, pred::is_positive{}}(0), true);
  test.expect("5 satisfies or_(zero, positive)", pred::or_{pred::is_zero{}, pred::is_positive{}}(5), true);
  test.expect("-3 fails or_(zero, positive)", pred::or_{pred::is_zero{}, pred::is_positive{}}(-3), false);
}

void test_xor_predicate() {
  auto &test = suite.get_test("xor_predicate");
  test.expect("3 satisfies xor_(positive, even)", pred::xor_{pred::is_positive{}, pred::is_even{}}(3), true);
  test.expect("-4 satisfies xor_(positive, even)", pred::xor_{pred::is_positive{}, pred::is_even{}}(-4), true);
  test.expect("4 fails xor_(positive, even)", pred::xor_{pred::is_positive{}, pred::is_even{}}(4), false);
  test.expect("-3 fails xor_(positive, even)", pred::xor_{pred::is_positive{}, pred::is_even{}}(-3), false);
}

void test_not_predicate() {
  auto &test = suite.get_test("not_predicate");
  auto not_positive = pred::not_{pred::is_positive{}};
  test.expect("-5 satisfies not_(positive)", not_positive(-5), true);
  test.expect("0 satisfies not_(positive)", not_positive(0), true);
  test.expect("5 fails not_(positive)", not_positive(5), false);
  auto not_even = pred::not_{pred::is_even{}};
  test.expect("3 satisfies not_(even)", not_even(3), true);
  test.expect("4 fails not_(even)", not_even(4), false);
}

void test_nand_and_nor_predicates() {
  auto &test = suite.get_test("nand_nor_predicates");
  auto nand_pred = pred::nand_{pred::is_positive{}, pred::is_even{}};
  test.expect("3 satisfies nand_(positive, even)", nand_pred(3), true);
  test.expect("-4 satisfies nand_(positive, even)", nand_pred(-4), true);
  test.expect("-3 satisfies nand_(positive, even)", nand_pred(-3), true);
  test.expect("4 fails nand_(positive, even)", nand_pred(4), false);
  auto nor_pred = pred::nor_{pred::is_zero{}, pred::is_positive{}};
  test.expect("-5 satisfies nor_(zero, positive)", nor_pred(-5), true);
  test.expect("0 fails nor_(zero, positive)", nor_pred(0), false);
  test.expect("5 fails nor_(zero, positive)", nor_pred(5), false);
}

void test_implies_predicate() {
  auto &test = suite.get_test("implies_predicate");
  auto implies_pred = pred::implies{pred::is_positive{}, [](int x) { return x > 0; }};
  test.expect("5 satisfies implies(positive, >0)", implies_pred(5), true);
  test.expect("0 satisfies implies(positive, >0)", implies_pred(0), true);
  test.expect("-5 satisfies implies(positive, >0)", implies_pred(-5), true);
}

void test_complex_combinations() {
  auto &test = suite.get_test("complex_combinations");
  auto complex_and = pred::and_{pred::or_{pred::is_positive{}, pred::is_zero{}}, pred::not_{pred::is_odd{}}};
  test.expect("4 satisfies complex and combination", complex_and(4), true);
  test.expect("0 satisfies complex and combination", complex_and(0), true);
  test.expect("3 fails complex and combination", complex_and(3), false);
  test.expect("-4 fails complex and combination", complex_and(-4), false);
  auto triple_or = pred::or_{pred::or_{pred::is_positive{}, pred::is_zero{}}, pred::is_even{}};
  test.expect("4 satisfies triple or", triple_or(4), true);
  test.expect("3 satisfies triple or", triple_or(3), true);
  test.expect("0 satisfies triple or", triple_or(0), true);
  test.expect("-4 satisfies triple or", triple_or(-4), true);
  test.expect("-3 fails triple or", triple_or(-3), false);
}

struct TestRunner {
  TestRunner() {
    test_and_predicate();
    test_or_predicate();
    test_xor_predicate();
    test_not_predicate();
    test_nand_and_nor_predicates();
    test_implies_predicate();
    test_complex_combinations();
  }
};

static TestRunner runner;

} // anonymous namespace
