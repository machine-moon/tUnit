#include "../include/evaluator_all.h"

using namespace tunit;
namespace pred = tunit::predicates;

int main()
{
    auto& suite = TestRunner::get_suite("Logical Predicates");

    // and_ tests
    auto positive_and_even = pred::and_{pred::is_positive{}, pred::is_even{}};
    suite.test("4 is_positive and is_even", [&](){ return positive_and_even(4); }());
    suite.test("3 is_not_positive_and_even", [&](){ return positive_and_even(3); }(), false);
    suite.test("-4 is_not_positive_and_even", [&](){ return positive_and_even(-4); }(), false);
    suite.test("-3 is_not_positive_and_even", [&](){ return positive_and_even(-3); }(), false);

    // or_ tests  
    auto zero_or_positive = pred::or_{pred::is_zero{}, pred::is_positive{}};
    suite.test("0 is_zero or is_positive", [&](){ return zero_or_positive(0); }());
    suite.test("5 is_zero or is_positive", [&](){ return zero_or_positive(5); }());
    suite.test("-3 is_not_zero_or_positive", [&](){ return zero_or_positive(-3); }(), false);

    // xor_ tests (exclusive or)
    auto positive_xor_even = pred::xor_{pred::is_positive{}, pred::is_even{}};
    suite.test("3 is_positive xor is_even", [&](){ return positive_xor_even(3); }());  // positive but not even
    suite.test("-4 is_positive xor is_even", [&](){ return positive_xor_even(-4); }()); // even but not positive
    suite.test("4 is_not_positive_xor_even", [&](){ return positive_xor_even(4); }(), false); // both true
    suite.test("-3 is_not_positive_xor_even", [&](){ return positive_xor_even(-3); }(), false); // both false

    // not_ tests
    auto not_positive = pred::not_{pred::is_positive{}};
    suite.test("-5 is not_positive", [&](){ return not_positive(-5); }());
    suite.test("0 is not_positive", [&](){ return not_positive(0); }());
    suite.test("5 is_positive", [&](){ return not_positive(5); }(), false);

    auto not_even = pred::not_{pred::is_even{}};
    suite.test("3 is not_even", [&](){ return not_even(3); }());
    suite.test("4 is_even", [&](){ return not_even(4); }(), false);

    // nand_ tests (not and)
    auto not_positive_and_even = pred::nand_{pred::is_positive{}, pred::is_even{}};
    suite.test("3 satisfies nand(positive, even)", [&](){ return not_positive_and_even(3); }());
    suite.test("-4 satisfies nand(positive, even)", [&](){ return not_positive_and_even(-4); }());
    suite.test("-3 satisfies nand(positive, even)", [&](){ return not_positive_and_even(-3); }());
    suite.test("4 fails nand(positive, even)", [&](){ return not_positive_and_even(4); }(), false);

    // nor_ tests (not or)
    auto not_zero_or_positive = pred::nor_{pred::is_zero{}, pred::is_positive{}};
    suite.test("-5 satisfies nor(zero, positive)", [&](){ return not_zero_or_positive(-5); }());
    suite.test("0 fails nor(zero, positive)", [&](){ return not_zero_or_positive(0); }(), false);
    suite.test("5 fails nor(zero, positive)", [&](){ return not_zero_or_positive(5); }(), false);

    // implies tests (A implies B = !A || B)
    auto positive_implies_greater_than_zero = pred::implies{pred::is_positive{}, pred::is_greater{}};
    // Need to use a wrapper for two-argument predicate
    auto test_implies = [&](int val) { 
        return pred::implies{pred::is_positive{}, [](auto x){ return x > 0; }}(val);
    };
    suite.test("5 positive implies > 0", [&](){ return test_implies(5); }());
    suite.test("0 satisfies positive implies > 0", [&](){ return test_implies(0); }()); // !positive is true
    suite.test("-5 satisfies positive implies > 0", [&](){ return test_implies(-5); }()); // !positive is true

    // Complex combinations
    auto complex_logic = pred::and_{
        pred::or_{pred::is_positive{}, pred::is_zero{}},
        pred::not_{pred::is_odd{}}
    };
    suite.test("4 satisfies (positive_or_zero) and not_odd", [&](){ return complex_logic(4); }());
    suite.test("0 satisfies (positive_or_zero) and not_odd", [&](){ return complex_logic(0); }());
    suite.test("3 fails (positive_or_zero) and not_odd", [&](){ return complex_logic(3); }(), false);
    suite.test("-4 fails (positive_or_zero) and not_odd", [&](){ return complex_logic(-4); }(), false);

    // Nested logical operations
    auto nested_logic = pred::or_{
        pred::and_{pred::is_positive{}, pred::is_even{}},
        pred::and_{pred::is_negative{}, pred::is_odd{}}
    };
    suite.test("4 satisfies nested (pos_and_even) or (neg_and_odd)", [&](){ return nested_logic(4); }());
    suite.test("-3 satisfies nested (pos_and_even) or (neg_and_odd)", [&](){ return nested_logic(-3); }());
    suite.test("3 fails nested logic", [&](){ return nested_logic(3); }(), false);
    suite.test("-4 fails nested logic", [&](){ return nested_logic(-4); }(), false);

    suite.suite_summary();
    return 0;
}
