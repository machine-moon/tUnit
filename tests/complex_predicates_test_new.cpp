#include "../include/evaluator_all.h"
#include <string>

using namespace tunit;
namespace pred = tunit::predicates;

int main()
{
    auto& suite = TestRunner::get_suite("Complex Predicates");

    // all_of tests
    auto is_positive_and_even = pred::all_of{pred::is_positive{}, pred::is_even{}};
    suite.test("4 is_positive and is_even", [&](){ return is_positive_and_even(4); }());
    suite.test("3 is_not_positive_and_even", [&](){ return is_positive_and_even(3); }(), false);
    suite.test("-4 is_not_positive_and_even", [&](){ return is_positive_and_even(-4); }(), false);

    // any_of tests  
    auto is_zero_or_positive = pred::any_of{pred::is_zero{}, pred::is_positive{}};
    suite.test("0 is_zero or is_positive", [&](){ return is_zero_or_positive(0); }());
    suite.test("5 is_zero or is_positive", [&](){ return is_zero_or_positive(5); }());
    suite.test("-3 is_not_zero_or_positive", [&](){ return is_zero_or_positive(-3); }(), false);

    // none_of tests
    auto is_neither_zero_nor_positive = pred::none_of{pred::is_zero{}, pred::is_positive{}};
    suite.test("-5 is_neither_zero_nor_positive", [&](){ return is_neither_zero_nor_positive(-5); }());
    suite.test("0 is_zero_or_positive", [&](){ return is_neither_zero_nor_positive(0); }(), false);
    suite.test("3 is_zero_or_positive", [&](){ return is_neither_zero_nor_positive(3); }(), false);

    // conditional tests
    auto conditional_pred = pred::conditional{
        pred::is_positive{},
        pred::is_even{},
        pred::is_odd{}
    };
    suite.test("4 positive->even, negative->odd", [&](){ return conditional_pred(4); }());
    suite.test("-3 positive->even, negative->odd", [&](){ return conditional_pred(-3); }());
    suite.test("3 fails positive->even", [&](){ return conditional_pred(3); }(), false);
    suite.test("-4 fails negative->odd", [&](){ return conditional_pred(-4); }(), false);

    // exactly_n_of tests
    auto exactly_2_of_3 = pred::exactly_n_of{2, pred::is_positive{}, pred::is_even{}, pred::is_greater{}};
    auto test_exactly_2 = [&](int val, int cmp) { return exactly_2_of_3(val, cmp); };
    suite.test("4 vs 2: exactly 2 of 3 predicates", [&](){ return test_exactly_2(4, 2); }());
    
    // at_least_n_of tests  
    auto at_least_1_of_2 = pred::at_least_n_of{1, pred::is_positive{}, pred::is_even{}};
    suite.test("4 satisfies at_least_1_of_2", [&](){ return at_least_1_of_2(4); }());
    suite.test("3 satisfies at_least_1_of_2", [&](){ return at_least_1_of_2(3); }());
    suite.test("-3 fails at_least_1_of_2", [&](){ return at_least_1_of_2(-3); }(), false);

    // at_most_n_of tests
    auto at_most_1_of_2 = pred::at_most_n_of{1, pred::is_positive{}, pred::is_even{}};
    suite.test("3 satisfies at_most_1_of_2", [&](){ return at_most_1_of_2(3); }());
    suite.test("-3 satisfies at_most_1_of_2", [&](){ return at_most_1_of_2(-3); }());
    suite.test("4 fails at_most_1_of_2", [&](){ return at_most_1_of_2(4); }(), false);

    // Complex nested predicates
    auto complex_nested = pred::any_of{
        pred::all_of{pred::is_positive{}, pred::is_even{}},
        pred::all_of{pred::is_negative{}, pred::is_odd{}}
    };
    suite.test("4 satisfies (positive_and_even) or (negative_and_odd)", [&](){ return complex_nested(4); }());
    suite.test("-3 satisfies (positive_and_even) or (negative_and_odd)", [&](){ return complex_nested(-3); }());
    suite.test("3 fails (positive_and_even) or (negative_and_odd)", [&](){ return complex_nested(3); }(), false);

    suite.suite_summary();
    return 0;
}
