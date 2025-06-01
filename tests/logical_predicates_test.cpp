<<<<<<< HEAD
#include "../include/evaluator_all.h"
#include <iostream>
#include <functional>

namespace pred = tunit::predicates;

int main()
{
    tunit::TestRunner runner("Logical Predicates Tests");

    std::cout << "=== Testing Logical Predicates ===" << std::endl;

    // ==================== and_ Predicate ====================
    std::cout << "\n--- and_ Predicate ---" << std::endl;

    {
        auto and_pred = pred::and_{pred::is_positive{}, pred::is_even{}};
        
        runner.test("and_: both true", and_pred(4));          // positive AND even
        runner.test("and_: first false", !and_pred(-4));      // negative AND even
        runner.test("and_: second false", !and_pred(3));      // positive AND odd
        runner.test("and_: both false", !and_pred(-3));       // negative AND odd
    }

    {
        auto and_binary = pred::and_{pred::is_greater{}, pred::is_less_equal{}};
        
        runner.test_evaluator("and_ binary: both true", 10, 5, and_binary, false);  // 10>5 AND 10<=5 = true AND false = false
        runner.test_evaluator("and_ binary: first true", 10, 5, 
            pred::and_{pred::is_greater{}, [](int a, int b) { return a > b-10; }}, true);  // 10>5 AND 10>-5
    }

    // ==================== or_ Predicate ====================
    std::cout << "\n--- or_ Predicate ---" << std::endl;

    {
        auto or_pred = pred::or_{pred::is_negative{}, pred::is_even{}};
        
        runner.test("or_: first true", or_pred(-3));          // negative OR even
        runner.test("or_: second true", or_pred(4));          // positive OR even
        runner.test("or_: both true", or_pred(-4));           // negative OR even
        runner.test("or_: both false", !or_pred(3));          // positive OR odd
    }

    {
        auto or_binary = pred::or_{pred::is_equal{}, pred::is_greater{}};
        
        runner.test_evaluator("or_ binary: equal", 5, 5, or_binary, true);      // 5==5 OR 5>5
        runner.test_evaluator("or_ binary: greater", 10, 5, or_binary, true);   // 10==5 OR 10>5
        runner.test_evaluator("or_ binary: neither", 3, 7, or_binary, false);   // 3==7 OR 3>7
    }

    // ==================== xor_ Predicate ====================
    std::cout << "\n--- xor_ Predicate ---" << std::endl;

    {
        auto xor_pred = pred::xor_{pred::is_positive{}, pred::is_even{}};
        
        runner.test("xor_: first true only", xor_pred(3));     // positive XOR even = true XOR false = true
        runner.test("xor_: second true only", xor_pred(-4));   // positive XOR even = false XOR true = true
        runner.test("xor_: both true", !xor_pred(4));          // positive XOR even = true XOR true = false
        runner.test("xor_: both false", !xor_pred(-3));        // positive XOR even = false XOR false = false
    }

    {
        auto xor_binary = pred::xor_{pred::is_equal{}, pred::is_greater{}};
        
        runner.test_evaluator("xor_ binary: equal only", 5, 5, xor_binary, true);    // equal XOR greater = true XOR false
        runner.test_evaluator("xor_ binary: greater only", 10, 5, xor_binary, true);  // equal XOR greater = false XOR true
        runner.test_evaluator("xor_ binary: neither", 3, 7, xor_binary, false);      // equal XOR greater = false XOR false
    }

    // ==================== not_ Predicate ====================
    std::cout << "\n--- not_ Predicate ---" << std::endl;

    {
        auto not_positive = pred::not_{pred::is_positive{}};
        
        runner.test("not_: invert true", not_positive(-5));    // NOT positive(-5) = NOT false = true
        runner.test("not_: invert false", !not_positive(5));   // NOT positive(5) = NOT true = false
        runner.test("not_: zero case", not_positive(0));       // NOT positive(0) = NOT false = true
    }

    {
        auto not_equal = pred::not_{pred::is_equal{}};
        
        runner.test_evaluator("not_ binary: not equal", 5, 3, not_equal, true);   // NOT(5==3) = NOT false = true
        runner.test_evaluator("not_ binary: equal", 5, 5, not_equal, false);      // NOT(5==5) = NOT true = false
    }

    // ==================== nand_ Predicate ====================
    std::cout << "\n--- nand_ Predicate ---" << std::endl;

    {
        auto nand_pred = pred::nand_{pred::is_positive{}, pred::is_even{}};
        
        runner.test("nand_: both true", !nand_pred(4));        // NOT(positive AND even) = NOT(true AND true) = false
        runner.test("nand_: first false", nand_pred(-4));      // NOT(positive AND even) = NOT(false AND true) = true
        runner.test("nand_: second false", nand_pred(3));      // NOT(positive AND even) = NOT(true AND false) = true
        runner.test("nand_: both false", nand_pred(-3));       // NOT(positive AND even) = NOT(false AND false) = true
    }

    {
        auto nand_binary = pred::nand_{pred::is_greater{}, pred::is_equal{}};
        
        runner.test_evaluator("nand_ binary: both false", 3, 7, nand_binary, true);    // NOT(false AND false) = true
        runner.test_evaluator("nand_ binary: first true", 10, 5, nand_binary, true);   // NOT(true AND false) = true
        runner.test_evaluator("nand_ binary: impossible case", 5, 5, nand_binary, true); // NOT(false AND true) = true (5 is not > 5)
    }

    // ==================== nor_ Predicate ====================
    std::cout << "\n--- nor_ Predicate ---" << std::endl;

    {
        auto nor_pred = pred::nor_{pred::is_negative{}, pred::is_even{}};
        
        runner.test("nor_: both false", nor_pred(3));          // NOT(negative OR even) = NOT(false OR false) = true
        runner.test("nor_: first true", !nor_pred(-3));        // NOT(negative OR even) = NOT(true OR false) = false
        runner.test("nor_: second true", !nor_pred(4));        // NOT(negative OR even) = NOT(false OR true) = false
        runner.test("nor_: both true", !nor_pred(-4));         // NOT(negative OR even) = NOT(true OR true) = false
    }

    {
        auto nor_binary = pred::nor_{pred::is_equal{}, pred::is_greater{}};
        
        runner.test_evaluator("nor_ binary: both false", 3, 7, nor_binary, true);     // NOT(false OR false) = true
        runner.test_evaluator("nor_ binary: first true", 5, 5, nor_binary, false);    // NOT(true OR false) = false
        runner.test_evaluator("nor_ binary: second true", 10, 5, nor_binary, false);  // NOT(false OR true) = false
    }

    // ==================== implies Predicate ====================
    std::cout << "\n--- implies Predicate ---" << std::endl;

    {
        // "if positive, then even" (positive → even)
        auto implies_pred = pred::implies{pred::is_positive{}, pred::is_even{}};
        
        runner.test("implies: antecedent true, consequent true", implies_pred(4));    // positive → even: true → true = true
        runner.test("implies: antecedent true, consequent false", !implies_pred(3));  // positive → even: true → false = false
        runner.test("implies: antecedent false, consequent true", implies_pred(-4));  // positive → even: false → true = true
        runner.test("implies: antecedent false, consequent false", implies_pred(-3)); // positive → even: false → false = true
    }

    {
        // "if greater, then not equal" (a > b → a ≠ b)
        auto implies_binary = pred::implies{pred::is_greater{}, pred::is_not_equal{}};
        
        runner.test_evaluator("implies binary: greater implies not equal", 10, 5, implies_binary, true);  // true → true = true
        runner.test_evaluator("implies binary: equal case", 5, 5, implies_binary, true);                  // false → false = true
        runner.test_evaluator("implies binary: less case", 3, 7, implies_binary, true);                   // false → true = true
    }

    // ==================== Complex Logical Combinations ====================
    std::cout << "\n--- Complex Logical Combinations ---" << std::endl;

    {
        // De Morgan's laws testing: NOT(A AND B) == (NOT A) OR (NOT B)
        auto left_side = pred::not_{pred::and_{pred::is_positive{}, pred::is_even{}}};
        auto right_side = pred::or_{pred::not_{pred::is_positive{}}, pred::not_{pred::is_even{}}};
        
        std::vector<int> test_values = {4, 3, -4, -3, 0};
        for (int val : test_values) {
            bool left_result = left_side(val);
            bool right_result = right_side(val);
            runner.test("De Morgan's law: NOT(A AND B) == (NOT A) OR (NOT B) for " + std::to_string(val), 
                       left_result == right_result);
        }
    }

    {
        // Complex business logic: "Valid adult user"
        // Must be: (age >= 18) AND ((has_license AND not_banned) OR is_admin)
        auto is_adult = [](int age) { return age >= 18; };
        auto has_license = [](int) { return true; };  // Simulate license check
        auto not_banned = [](int) { return true; };   // Simulate ban check
        auto is_admin = [](int age) { return age >= 65; }; // Seniors are auto-admin
        
        auto valid_user = pred::and_{
            is_adult,
            pred::or_{
                pred::and_{has_license, not_banned},
                is_admin
            }
        };
        
        runner.test("complex logic: young adult", valid_user(25));
        runner.test("complex logic: senior", valid_user(70));
        runner.test("complex logic: minor", !valid_user(16));
    }

    // ==================== Chained Logical Operations ====================
    std::cout << "\n--- Chained Logical Operations ---" << std::endl;

    {
        // Chain multiple logical operations
        auto chained = pred::and_{
            pred::or_{pred::is_positive{}, pred::is_zero{}},  // non-negative
            pred::not_{pred::is_odd{}},                       // even
            pred::implies{
                [](int x) { return x > 10; },                 // if > 10
                [](int x) { return x < 100; }                 // then < 100
            }
        };
        
        runner.test("chained: valid small even", chained(8));   // non-negative, even, (8>10)→(8<100) = false→true = true
        runner.test("chained: valid medium even", chained(50)); // non-negative, even, (50>10)→(50<100) = true→true = true
        runner.test("chained: invalid large even", !chained(150)); // non-negative, even, (150>10)→(150<100) = true→false = false
        runner.test("chained: invalid odd", !chained(7));      // non-negative, NOT even = false
    }

    // ==================== Performance Tests ====================
    std::cout << "\n--- Performance Tests ---" << std::endl;

    {
        auto simple_and = pred::and_{pred::is_positive{}, pred::is_even{}};
        runner.benchmark("simple and_ performance", [&]() {
            return simple_and(42);
        }, 100000);

        auto complex_logic = pred::and_{
            pred::or_{pred::is_positive{}, pred::is_zero{}},
            pred::not_{pred::implies{pred::is_even{}, pred::is_greater{}}},
            pred::xor_{pred::is_odd{}, [](int x) { return x > 10; }}
        };
        runner.benchmark("complex logical combination", [&]() {
            return complex_logic(42);
        }, 10000);
    }

    // ==================== Edge Cases ====================
    std::cout << "\n--- Edge Cases ---" << std::endl;

    {
        // Test short-circuiting behavior (conceptual - actual behavior depends on implementation)
        auto short_circuit_and = pred::and_{
            [](int x) { return x > 0; },
            [](int x) { std::cout << "Second predicate evaluated\n"; return x < 100; }
        };
        
        // This would show short-circuiting if implemented
        runner.test("short circuit: false first (no output expected)", !short_circuit_and(-5));
        runner.test("short circuit: true first (output expected)", short_circuit_and(50));
    }

    {
        // Nested logical operations
        auto deeply_nested = pred::and_{
            pred::or_{
                pred::and_{pred::is_positive{}, pred::is_even{}},
                pred::not_{pred::is_negative{}}
            },
            pred::implies{
                pred::xor_{pred::is_odd{}, [](int x) { return x > 50; }},
                pred::nor_{[](int x) { return x < 0; }, [](int x) { return x > 1000; }}
            }
        };
        
        runner.test("deeply nested: complex case", deeply_nested(42));
    }

    {
        // Tautologies and contradictions
        auto tautology = pred::or_{pred::is_positive{}, pred::not_{pred::is_positive{}}};
        auto contradiction = pred::and_{pred::is_positive{}, pred::not_{pred::is_positive{}}};
        
        runner.test("tautology: always true (positive)", tautology(5));
        runner.test("tautology: always true (negative)", tautology(-5));
        runner.test("tautology: always true (zero)", tautology(0));
        
        runner.test("contradiction: always false (positive)", !contradiction(5));
        runner.test("contradiction: always false (negative)", !contradiction(-5));
        runner.test("contradiction: always false (zero)", !contradiction(0));
    }

    runner.summary();
    return runner.all_passed() ? 0 : 1;
=======
#include "../include/tunit.h"

namespace pred = tunit::predicates;

int main() {
  auto& suite = tunit::TestRunner::get_suite("Logical Predicates");
  suite.suite_header();

  // Test and_ predicate combinator
  auto and_pred = pred::and_{pred::is_positive{}, pred::is_even{}};
  suite.test("4 satisfies and_(positive, even)", and_pred(4));
  suite.test("3 fails and_(positive, even)", and_pred(3), false);
  suite.test("-4 fails and_(positive, even)", and_pred(-4), false);
  suite.test("-3 fails and_(positive, even)", and_pred(-3), false);

  // Test or_ predicate combinator
  auto or_pred = pred::or_{pred::is_zero{}, pred::is_positive{}};
  suite.test("0 satisfies or_(zero, positive)", or_pred(0));
  suite.test("5 satisfies or_(zero, positive)", or_pred(5));
  suite.test("-3 fails or_(zero, positive)", or_pred(-3), false);

  // Test xor_ predicate combinator
  auto xor_pred = pred::xor_{pred::is_positive{}, pred::is_even{}};
  suite.test("3 satisfies xor_(positive, even)", xor_pred(3));
  suite.test("-4 satisfies xor_(positive, even)", xor_pred(-4));
  suite.test("4 fails xor_(positive, even)", xor_pred(4), false);
  suite.test("-3 fails xor_(positive, even)", xor_pred(-3), false);

  // Test not_ predicate combinator
  auto not_positive = pred::not_{pred::is_positive{}};
  suite.test("-5 satisfies not_(positive)", not_positive(-5));
  suite.test("0 satisfies not_(positive)", not_positive(0));
  suite.test("5 fails not_(positive)", not_positive(5), false);

  auto not_even = pred::not_{pred::is_even{}};
  suite.test("3 satisfies not_(even)", not_even(3));
  suite.test("4 fails not_(even)", not_even(4), false);

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

  // Test implies predicate combinator
  auto implies_pred = pred::implies{pred::is_positive{}, [](int x) { return x > 0; }};
  suite.test("5 satisfies implies(positive, >0)", implies_pred(5));
  suite.test("0 satisfies implies(positive, >0)", implies_pred(0));
  suite.test("-5 satisfies implies(positive, >0)", implies_pred(-5));

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

  suite.suite_summary();
  return 0;
>>>>>>> 7f4cf2b (uff)
}
