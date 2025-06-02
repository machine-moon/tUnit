#include <string>

#include "tsuite.h"

namespace {
namespace pred = tunit::predicates;

void test_all_of_predicate() {
  auto &suite = tunit::Runner::get_suite("Complex Predicates");

  // Test all_of predicate
  auto all_pred = pred::all_of{pred::is_positive{}, pred::is_even{}};
  suite.test("4 satisfies all_of(positive, even)", all_pred(4));
  suite.test("3 fails all_of(positive, even)", all_pred(3), false);
  suite.test("-4 fails all_of(positive, even)", all_pred(-4), false);
}

void test_any_of_predicate() {
  auto &suite = tunit::Runner::get_suite("Complex Predicates");

  // Test any_of predicate
  auto any_pred = pred::any_of(pred::is_positive{}, pred::is_even{});
  suite.test("4 satisfies any_of(positive, even)", any_pred(4));
  suite.test("3 satisfies any_of(positive, even)", any_pred(3));
  suite.test("-4 satisfies any_of(positive, even)", any_pred(-4));
  suite.test("-3 fails any_of(positive, even)", any_pred(-3), false);
}

void test_none_of_predicate() {
  auto &suite = tunit::Runner::get_suite("Complex Predicates");

  // Test none_of predicate
  auto none_pred = pred::none_of(pred::is_positive{}, pred::is_even{});
  suite.test("-3 satisfies none_of(positive, even)", none_pred(-3));
  suite.test("4 fails none_of(positive, even)", none_pred(4), false);
  suite.test("3 fails none_of(positive, even)", none_pred(3), false);
}

void test_conditional_predicate() {
  auto &suite = tunit::Runner::get_suite("Complex Predicates");

  // Test conditional predicate
  auto cond_pred =
      pred::conditional(pred::is_positive{}, pred::is_even{}, pred::is_odd{});
  suite.test("4 satisfies conditional(positive->even, else->odd)",
             cond_pred(4));
  suite.test("-3 satisfies conditional(positive->even, else->odd)",
             cond_pred(-3));
  suite.test("3 fails conditional(positive->even, else->odd)", cond_pred(3),
             false);
  suite.test("-4 fails conditional(positive->even, else->odd)", cond_pred(-4),
             false);
}

void test_exactly_n_of_predicate() {
  auto &suite = tunit::Runner::get_suite("Complex Predicates");

  // Test exactly_n_of predicate
  auto exactly_pred =
      pred::exactly_n_of(1, pred::is_positive{}, pred::is_even{});
  suite.test("3 satisfies exactly_1_of(positive, even)", exactly_pred(3));
  suite.test("-4 satisfies exactly_1_of(positive, even)", exactly_pred(-4));
  suite.test("4 fails exactly_1_of(positive, even)", exactly_pred(4), false);
  suite.test("-3 fails exactly_1_of(positive, even)", exactly_pred(-3), false);
}

void test_at_least_n_of_predicate() {
  auto &suite = tunit::Runner::get_suite("Complex Predicates");

  // Test at_least_n_of predicate
  auto at_least_pred =
      pred::at_least_n_of(1, pred::is_positive{}, pred::is_even{});
  suite.test("4 satisfies at_least_1_of(positive, even)", at_least_pred(4));
  suite.test("3 satisfies at_least_1_of(positive, even)", at_least_pred(3));
  suite.test("-3 fails at_least_1_of(positive, even)", at_least_pred(-3),
             false);
}

void test_at_most_n_of_predicate() {
  auto &suite = tunit::Runner::get_suite("Complex Predicates");

  // Test at_most_n_of predicate
  auto at_most_pred =
      pred::at_most_n_of(1, pred::is_positive{}, pred::is_even{});
  suite.test("3 satisfies at_most_1_of(positive, even)", at_most_pred(3));
  suite.test("-3 satisfies at_most_1_of(positive, even)", at_most_pred(-3));
  suite.test("4 fails at_most_1_of(positive, even)", at_most_pred(4), false);
}

// Static initialization to run tests
struct ComplexPredicatesTestRunner {
  ComplexPredicatesTestRunner() {
    test_all_of_predicate();
    test_any_of_predicate();
    test_none_of_predicate();
    test_conditional_predicate();
    test_exactly_n_of_predicate();
    test_at_least_n_of_predicate();
    test_at_most_n_of_predicate();
  }
};

static ComplexPredicatesTestRunner runner;

} // anonymous namespace
