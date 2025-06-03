#include "tunit.h"
#include <string>

namespace
{
namespace pred = tunit::predicates;

auto &suite = tunit::Orchestrator::instance().get_suite("Complex Predicates");

void test_all_of_predicate()
{
  auto &test = suite.get_test("all_of");
  test.expect("4 satisfies all_of(positive, even)", pred::all_of{pred::is_positive{}, pred::is_even{}}(4), true);
  test.expect("3 fails all_of(positive, even)", pred::all_of{pred::is_positive{}, pred::is_even{}}(3), false);
  test.expect("-4 fails all_of(positive, even)", pred::all_of{pred::is_positive{}, pred::is_even{}}(-4), false);
}

void test_any_of_predicate()
{
  auto &test = suite.get_test("any_of");
  test.expect("4 satisfies any_of(positive, even)", pred::any_of(pred::is_positive{}, pred::is_even{})(4), true);
  test.expect("3 satisfies any_of(positive, even)", pred::any_of(pred::is_positive{}, pred::is_even{})(3), true);
  test.expect("-4 satisfies any_of(positive, even)", pred::any_of(pred::is_positive{}, pred::is_even{})(-4), true);
  test.expect("-3 fails any_of(positive, even)", pred::any_of(pred::is_positive{}, pred::is_even{})(-3), false);
}

void test_none_of_predicate()
{
  auto &test = suite.get_test("none_of");
  test.expect("-3 satisfies none_of(positive, even)", pred::none_of(pred::is_positive{}, pred::is_even{})(-3), true);
  test.expect("4 fails none_of(positive, even)", pred::none_of(pred::is_positive{}, pred::is_even{})(4), false);
  test.expect("3 fails none_of(positive, even)", pred::none_of(pred::is_positive{}, pred::is_even{})(3), false);
}

void test_conditional_predicate()
{
  auto &test = suite.get_test("conditional");
  test.expect("4 satisfies conditional(positive->even, else->odd)", pred::conditional(pred::is_positive{}, pred::is_even{}, pred::is_odd{})(4), true);
  test.expect("-3 satisfies conditional(positive->even, else->odd)", pred::conditional(pred::is_positive{}, pred::is_even{}, pred::is_odd{})(-3), true);
  test.expect("3 fails conditional(positive->even, else->odd)", pred::conditional(pred::is_positive{}, pred::is_even{}, pred::is_odd{})(3), false);
  test.expect("-4 fails conditional(positive->even, else->odd)", pred::conditional(pred::is_positive{}, pred::is_even{}, pred::is_odd{})(-4), false);
}

void test_exactly_n_of_predicate()
{
  auto &test = suite.get_test("exactly_n_of");
  test.expect("3 satisfies exactly_1_of(positive, even)", pred::exactly_n_of(1, pred::is_positive{}, pred::is_even{})(3), true);
  test.expect("-4 satisfies exactly_1_of(positive, even)", pred::exactly_n_of(1, pred::is_positive{}, pred::is_even{})(-4), true);
  test.expect("4 fails exactly_1_of(positive, even)", pred::exactly_n_of(1, pred::is_positive{}, pred::is_even{})(4), false);
  test.expect("-3 fails exactly_1_of(positive, even)", pred::exactly_n_of(1, pred::is_positive{}, pred::is_even{})(-3), false);
}

void test_at_least_n_of_predicate()
{
  auto &test = suite.get_test("at_least_n_of");
  test.expect("4 satisfies at_least_1_of(positive, even)", pred::at_least_n_of(1, pred::is_positive{}, pred::is_even{})(4), true);
  test.expect("3 satisfies at_least_1_of(positive, even)", pred::at_least_n_of(1, pred::is_positive{}, pred::is_even{})(3), true);
  test.expect("-3 fails at_least_1_of(positive, even)", pred::at_least_n_of(1, pred::is_positive{}, pred::is_even{})(-3), false);
}

void test_at_most_n_of_predicate()
{
  auto &test = suite.get_test("at_most_n_of");
  test.expect("3 satisfies at_most_1_of(positive, even)", pred::at_most_n_of(1, pred::is_positive{}, pred::is_even{})(3), true);
  test.expect("-3 satisfies at_most_1_of(positive, even)", pred::at_most_n_of(1, pred::is_positive{}, pred::is_even{})(-3), true);
  test.expect("4 fails at_most_1_of(positive, even)", pred::at_most_n_of(1, pred::is_positive{}, pred::is_even{})(4), false);
}

struct ComplexPredicatesTestRunner
{
  ComplexPredicatesTestRunner()
  {
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
