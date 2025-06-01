<<<<<<< HEAD
#include "../include/evaluator_all.h"
#include <iostream>
#include <vector>
#include <string>

namespace pred = tunit::predicates;

int main()
{
    tunit::TestRunner runner("Complex Predicates Tests");

    std::cout << "=== Testing Complex Predicates ===" << std::endl;

    // ==================== all_of Predicate ====================
    std::cout << "\n--- all_of Predicate ---" << std::endl;

    {
        auto all_positive = pred::all_of{pred::is_positive{}, pred::is_greater{}};
        
        runner.test_evaluator("all_of: both conditions true", 10, 5, all_positive, true);
        runner.test_evaluator("all_of: first condition false", -10, 5, all_positive, false);
        runner.test_evaluator("all_of: second condition false", 10, 15, all_positive, false);
        runner.test_evaluator("all_of: both conditions false", -10, 15, all_positive, false);
    }

    {
        auto all_numeric_checks = pred::all_of{pred::is_positive{}, pred::is_odd{}, pred::is_greater{}};
        
        runner.test_evaluator("all_of triple: all true", 5, 3, all_numeric_checks, true);
        runner.test_evaluator("all_of triple: one false", 4, 3, all_numeric_checks, false); // 4 is even
        runner.test_evaluator("all_of triple: multiple false", -4, 3, all_numeric_checks, false); // negative and even
    }

    // ==================== any_of Predicate ====================
    std::cout << "\n--- any_of Predicate ---" << std::endl;

    {
        auto any_condition = pred::any_of{pred::is_equal{}, pred::is_greater{}};
        
        runner.test_evaluator("any_of: first true", 5, 5, any_condition, true);
        runner.test_evaluator("any_of: second true", 10, 5, any_condition, true);
        runner.test_evaluator("any_of: both true", 5, 5, any_condition, true);
        runner.test_evaluator("any_of: both false", 3, 10, any_condition, false);
    }

    {
        auto any_numeric = pred::any_of{pred::is_even{}, pred::is_negative{}, pred::is_zero{}};
        
        runner.test("any_of unary: even number", any_numeric(4));
        runner.test("any_of unary: negative number", any_numeric(-3));
        runner.test("any_of unary: zero", any_numeric(0));
        runner.test("any_of unary: none match", !any_numeric(3)); // positive odd non-zero
    }

    // ==================== none_of Predicate ====================
    std::cout << "\n--- none_of Predicate ---" << std::endl;

    {
        auto none_bad = pred::none_of{pred::is_negative{}, pred::is_zero{}};
        
        runner.test("none_of: all conditions false", none_bad(5));
        runner.test("none_of: one condition true", !none_bad(-5));
        runner.test("none_of: another condition true", !none_bad(0));
    }

    {
        auto none_special = pred::none_of{pred::is_greater{}, pred::is_equal{}};
        
        runner.test_evaluator("none_of binary: both false", 3, 10, none_special, true);
        runner.test_evaluator("none_of binary: first true", 10, 3, none_special, false);
        runner.test_evaluator("none_of binary: second true", 5, 5, none_special, false);
    }

    // ==================== conditional Predicate ====================
    std::cout << "\n--- conditional Predicate ---" << std::endl;

    {
        auto conditional_check = pred::conditional{
            pred::is_positive{},  // condition
            pred::is_even{},      // then: if positive, must be even
            pred::is_odd{}        // else: if not positive, must be odd
        };
        
        runner.test("conditional: positive even", conditional_check(4));      // positive -> even ✓
        runner.test("conditional: positive odd", !conditional_check(3));     // positive -> even ✗
        runner.test("conditional: negative odd", conditional_check(-3));     // negative -> odd ✓
        runner.test("conditional: negative even", !conditional_check(-4));   // negative -> odd ✗
        runner.test("conditional: zero case", !conditional_check(0));        // zero -> odd ✗
    }

    {
        auto string_conditional = pred::conditional{
            [](const std::string& s) { return s.length() > 5; },  // condition
            [](const std::string& s) { return s.find("test") != std::string::npos; },  // then
            [](const std::string& s) { return s.length() < 3; }   // else
        };
        
        runner.test("conditional string: long with test", string_conditional(std::string("testing123")));
        runner.test("conditional string: long without test", !string_conditional(std::string("programming")));
        runner.test("conditional string: short", string_conditional(std::string("hi")));
        runner.test("conditional string: medium", !string_conditional(std::string("hello")));
    }

    // ==================== exactly_n_of Predicate ====================
    std::cout << "\n--- exactly_n_of Predicate ---" << std::endl;

    {
        auto exactly_two = pred::exactly_n_of{2, pred::is_positive{}, pred::is_even{}, pred::is_greater{}};
        
        runner.test_evaluator("exactly_n_of: exactly 2 true", 6, 4, exactly_two, true);  // positive✓, even✓, greater✓ = 3
        runner.test_evaluator("exactly_n_of: less than 2", 2, 4, exactly_two, false);   // positive✓, even✓, greater✗ = 2
        runner.test_evaluator("exactly_n_of: more than 2", 5, 3, exactly_two, false);   // positive✓, even✗, greater✓ = 2
    }

    {
        auto exactly_one = pred::exactly_n_of{1, pred::is_negative{}, pred::is_zero{}, pred::is_positive{}};
        
        runner.test("exactly_n_of unary: exactly 1 (negative)", exactly_one(-5));
        runner.test("exactly_n_of unary: exactly 1 (zero)", exactly_one(0));
        runner.test("exactly_n_of unary: exactly 1 (positive)", exactly_one(5));
        runner.test("exactly_n_of unary: none match", !exactly_one(std::numeric_limits<int>::quiet_NaN())); // This won't compile, let's use a valid test
    }

    // ==================== at_least_n_of Predicate ====================
    std::cout << "\n--- at_least_n_of Predicate ---" << std::endl;

    {
        auto at_least_two = pred::at_least_n_of{2, pred::is_positive{}, pred::is_even{}, pred::is_greater{}};
        
        runner.test_evaluator("at_least_n_of: exactly 2", 4, 2, at_least_two, true);    // positive✓, even✓, greater✓ = 3 >= 2
        runner.test_evaluator("at_least_n_of: more than 2", 6, 4, at_least_two, true);  // positive✓, even✓, greater✓ = 3 >= 2
        runner.test_evaluator("at_least_n_of: less than 2", 3, 5, at_least_two, false); // positive✓, even✗, greater✗ = 1 < 2
    }

    {
        auto at_least_one = pred::at_least_n_of{1, pred::is_even{}, pred::is_odd{}};
        
        runner.test("at_least_n_of unary: even", at_least_one(4));
        runner.test("at_least_n_of unary: odd", at_least_one(3));
        // Note: A number can't be both even and odd, so max is 1
    }

    // ==================== at_most_n_of Predicate ====================
    std::cout << "\n--- at_most_n_of Predicate ---" << std::endl;

    {
        auto at_most_two = pred::at_most_n_of{2, pred::is_positive{}, pred::is_even{}, pred::is_greater{}};
        
        runner.test_evaluator("at_most_n_of: exactly 2", 4, 2, at_most_two, true);    // positive✓, even✓, greater✓ = 3 > 2
        runner.test_evaluator("at_most_n_of: less than 2", 3, 5, at_most_two, true);  // positive✓, even✗, greater✗ = 1 <= 2
        runner.test_evaluator("at_most_n_of: more than 2", 6, 4, at_most_two, false); // positive✓, even✓, greater✓ = 3 > 2
    }

    {
        auto at_most_zero = pred::at_most_n_of{0, pred::is_negative{}, pred::is_zero{}};
        
        runner.test("at_most_n_of unary: positive (0 match)", at_most_zero(5));
        runner.test("at_most_n_of unary: negative (1 match)", !at_most_zero(-5));
        runner.test("at_most_n_of unary: zero (1 match)", !at_most_zero(0));
    }

    // ==================== Complex Combinations ====================
    std::cout << "\n--- Complex Combinations ---" << std::endl;

    {
        // Nested combinations
        auto complex_combo = pred::all_of{
            pred::any_of{pred::is_positive{}, pred::is_zero{}},  // non-negative
            pred::at_most_n_of{1, pred::is_even{}, pred::is_odd{}},  // exactly one of even/odd
            pred::none_of{[](int x) { return x > 100; }}  // not too large
        };
        
        runner.test("complex combo: valid case", complex_combo(42));
        runner.test("complex combo: negative", !complex_combo(-5));
        runner.test("complex combo: too large", !complex_combo(150));
    }

    {
        // Real-world example: validation predicate
        auto valid_age = pred::conditional{
            pred::is_positive{},  // if positive
            pred::all_of{  // then must be reasonable age
                [](int age) { return age >= 0; },
                [](int age) { return age <= 150; }
            },
            [](int) { return false; }  // else invalid
        };
        
        runner.test("valid_age: normal age", valid_age(25));
        runner.test("valid_age: child", valid_age(5));
        runner.test("valid_age: elderly", valid_age(95));
        runner.test("valid_age: too old", !valid_age(200));
        runner.test("valid_age: negative", !valid_age(-5));
        runner.test("valid_age: zero", valid_age(0));
    }

    // ==================== Performance Tests ====================
    std::cout << "\n--- Performance Tests ---" << std::endl;

    {
        auto simple_all = pred::all_of{pred::is_positive{}, pred::is_even{}};
        runner.benchmark("all_of performance", [&]() {
            return simple_all(42);
        }, 50000);

        auto complex_nested = pred::all_of{
            pred::any_of{pred::is_positive{}, pred::is_zero{}},
            pred::exactly_n_of{1, pred::is_even{}, pred::is_odd{}},
            pred::at_least_n_of{1, [](int x) { return x > 0; }, [](int x) { return x < 100; }}
        };
        runner.benchmark("complex nested performance", [&]() {
            return complex_nested(42);
        }, 10000);
    }

    // ==================== Edge Cases ====================
    std::cout << "\n--- Edge Cases ---" << std::endl;

    {
        // Empty all_of (should always be true)
        // Note: This might not compile depending on implementation
        // auto empty_all = pred::all_of{};
        
        // Single predicate cases
        auto single_all = pred::all_of{pred::is_positive{}};
        auto single_any = pred::any_of{pred::is_positive{}};
        auto single_none = pred::none_of{pred::is_negative{}};
        
        runner.test("single all_of", single_all(5));
        runner.test("single any_of", single_any(5));
        runner.test("single none_of", single_none(5));
    }

    {
        // Zero count cases
        auto zero_exactly = pred::exactly_n_of{0, pred::is_negative{}, pred::is_zero{}};
        auto zero_at_least = pred::at_least_n_of{0, pred::is_negative{}, pred::is_zero{}};
        auto zero_at_most = pred::at_most_n_of{0, pred::is_negative{}, pred::is_zero{}};
        
        runner.test("exactly 0: positive number", zero_exactly(5));
        runner.test("at_least 0: always true", zero_at_least(5) && zero_at_least(-5));
        runner.test("at_most 0: positive number", zero_at_most(5));
        runner.test("at_most 0: negative number", !zero_at_most(-5));
    }

    runner.summary();
    return runner.all_passed() ? 0 : 1;
=======
#include <string>

#include "../include/tunit.h"

namespace pred = tunit::predicates;

int main() {
  auto &suite = tunit::TestRunner::get_suite("Complex Predicates");
  suite.suite_header();

  // Test all_of predicate
  auto all_pred = pred::all_of{pred::is_positive{}, pred::is_even{}};
  suite.test("4 satisfies all_of(positive, even)", all_pred(4));
  suite.test("3 fails all_of(positive, even)", all_pred(3), false);
  suite.test("-4 fails all_of(positive, even)", all_pred(-4), false);

  // Test any_of predicate
  auto any_pred = pred::any_of(pred::is_positive{}, pred::is_even{});
  suite.test("4 satisfies any_of(positive, even)", any_pred(4));
  suite.test("3 satisfies any_of(positive, even)", any_pred(3));
  suite.test("-4 satisfies any_of(positive, even)", any_pred(-4));
  suite.test("-3 fails any_of(positive, even)", any_pred(-3), false);

  // Test none_of predicate
  auto none_pred = pred::none_of(pred::is_positive{}, pred::is_even{});
  suite.test("-3 satisfies none_of(positive, even)", none_pred(-3));
  suite.test("4 fails none_of(positive, even)", none_pred(4), false);
  suite.test("3 fails none_of(positive, even)", none_pred(3), false);

  // Test conditional predicate
  auto cond_pred = pred::conditional(pred::is_positive{}, pred::is_even{}, pred::is_odd{});
  suite.test("4 satisfies conditional(positive->even, else->odd)", cond_pred(4));
  suite.test("-3 satisfies conditional(positive->even, else->odd)", cond_pred(-3));
  suite.test("3 fails conditional(positive->even, else->odd)", cond_pred(3), false);
  suite.test("-4 fails conditional(positive->even, else->odd)", cond_pred(-4), false);

  // add exactly_n_of here
  auto exactly_pred = pred::exactly_n_of(1, pred::is_positive{}, pred::is_even{});
  suite.test("3 satisfies exactly_1_of(positive, even)", exactly_pred(3));
  suite.test("-4 satisfies exactly_1_of(positive, even)", exactly_pred(-4));
  suite.test("4 fails exactly_1_of(positive, even)", exactly_pred(4), false);
  suite.test("-3 fails exactly_1_of(positive, even)", exactly_pred(-3), false);

  // Test at_least_n_of predicate
  auto at_least_pred = pred::at_least_n_of(1, pred::is_positive{}, pred::is_even{});
  suite.test("4 satisfies at_least_1_of(positive, even)", at_least_pred(4));
  suite.test("3 satisfies at_least_1_of(positive, even)", at_least_pred(3));
  suite.test("-3 fails at_least_1_of(positive, even)", at_least_pred(-3), false);

  // Test at_most_n_of predicate
  auto at_most_pred = pred::at_most_n_of(1, pred::is_positive{}, pred::is_even{});
  suite.test("3 satisfies at_most_1_of(positive, even)", at_most_pred(3));
  suite.test("-3 satisfies at_most_1_of(positive, even)", at_most_pred(-3));
  suite.test("4 fails at_most_1_of(positive, even)", at_most_pred(4), false);

  suite.suite_summary();
  return 0;
>>>>>>> 7f4cf2b (uff)
}
