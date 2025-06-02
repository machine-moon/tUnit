#include "tunit.h"

namespace {
namespace pred = tunit::predicates;

void test_and_predicate() {
  auto& suite = tunit::Runner::get_suite("Logical Predicates");

  // Test and_ predicate combinator
  auto and_pred = pred::and_{pred::is_positive{}, pred::is_even{}};
  suite.test("4 satisfies and_(positive, even)", and_pred(4));
  suite.test("3 fails and_(positive, even)", and_pred(3), false);
  suite.test("-4 fails and_(positive, even)", and_pred(-4), false);
  suite.test("-3 fails and_(positive, even)", and_pred(-3), false);
}

void test_or_predicate() {
  auto &suite = tunit::Runner::get_suite("Logical Predicates");

  // Test or_ predicate combinator
  auto or_pred = pred::or_{pred::is_zero{}, pred::is_positive{}};
  suite.test("0 satisfies or_(zero, positive)", or_pred(0));
  suite.test("5 satisfies or_(zero, positive)", or_pred(5));
  suite.test("-3 fails or_(zero, positive)", or_pred(-3), false);
}

void test_xor_predicate() {
  auto &suite = tunit::Runner::get_suite("Logical Predicates");

  // Test xor_ predicate combinator
  auto xor_pred = pred::xor_{pred::is_positive{}, pred::is_even{}};
  suite.test("3 satisfies xor_(positive, even)", xor_pred(3));
  suite.test("-4 satisfies xor_(positive, even)", xor_pred(-4));
  suite.test("4 fails xor_(positive, even)", xor_pred(4), false);
  suite.test("-3 fails xor_(positive, even)", xor_pred(-3), false);
}

void test_not_predicate() {
  auto &suite = tunit::Runner::get_suite("Logical Predicates");

  // Test not_ predicate combinator
  auto not_positive = pred::not_{pred::is_positive{}};
  suite.test("-5 satisfies not_(positive)", not_positive(-5));
  suite.test("0 satisfies not_(positive)", not_positive(0));
  suite.test("5 fails not_(positive)", not_positive(5), false);

  auto not_even = pred::not_{pred::is_even{}};
  suite.test("3 satisfies not_(even)", not_even(3));
  suite.test("4 fails not_(even)", not_even(4), false);
}

void test_nand_and_nor_predicates() {
  auto &suite = tunit::Runner::get_suite("Logical Predicates");

  // Test nand_ predicate combinator
  auto nand_pred = pred::nand_{pred::is_positive{}, pred::is_even{}};
  suite.test("3 satisfies nand_(positive, even)", nand_pred(3));
  suite.test("-4 satisfies nand_(positive, even)", nand_pred(-4));
  suite.test("-3 satisfies nand_(positive, even)", nand_pred(-3));
  suite.test("4 fails nand_(positive, even)", nand_pred(4), false);

  // Test nor_ predicate combinator
  auto nor_pred = pred::nor_{pred::is_zero{}, pred::is_positive{}};
  suite.test("-5 satisfies nor_(zero, positive)", nor_pred(-5));
  suite.test("0 fails nor_(zero, positive)", nor_pred(0), false);
  suite.test("5 fails nor_(zero, positive)", nor_pred(5), false);
}

void test_implies_predicate() {
  auto &suite = tunit::Runner::get_suite("Logical Predicates");

  // Test implies predicate combinator
  auto implies_pred = pred::implies{pred::is_positive{}, [](int x) { return x > 0; }};
  suite.test("5 satisfies implies(positive, >0)", implies_pred(5));
  suite.test("0 satisfies implies(positive, >0)", implies_pred(0));
  suite.test("-5 satisfies implies(positive, >0)", implies_pred(-5));
}

void test_complex_combinations() {
  auto &suite = tunit::Runner::get_suite("Logical Predicates");

  // Test nested logical combinations
  auto complex_and = pred::and_{pred::or_{pred::is_positive{}, pred::is_zero{}}, pred::not_{pred::is_odd{}}};
  suite.test("4 satisfies complex and combination", complex_and(4));
  suite.test("0 satisfies complex and combination", complex_and(0));
  suite.test("3 fails complex and combination", complex_and(3), false);
  suite.test("-4 fails complex and combination", complex_and(-4), false);

  // Test triple combination
  auto triple_or = pred::or_{pred::or_{pred::is_positive{}, pred::is_zero{}}, pred::is_even{}};
  suite.test("4 satisfies triple or", triple_or(4));
  suite.test("3 satisfies triple or", triple_or(3));
  suite.test("0 satisfies triple or", triple_or(0));
  suite.test("-4 satisfies triple or", triple_or(-4));
  suite.test("-3 fails triple or", triple_or(-3), false);
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
