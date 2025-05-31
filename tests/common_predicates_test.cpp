#include "../include/evaluator_all.h"
#include <iostream>
#include <string>
#include <string_view>

namespace pred = tunit::predicates;

int main()
{
    tunit::TestRunner runner("Common Predicates Tests");

    std::cout << "=== Testing Common Predicates ===" << std::endl;

    // ==================== Basic Comparison Predicates ====================
    std::cout << "\n--- Basic Comparison Predicates ---" << std::endl;
    
    {
        runner.test_evaluator("is_equal: integers", 5, 5, pred::is_equal{}, true);
        runner.test_evaluator("is_equal: different integers", 5, 3, pred::is_equal{}, false);
        runner.test_evaluator("is_equal: doubles", 3.14, 3.14, pred::is_equal{}, true);
        runner.test_evaluator("is_equal: strings", std::string("hello"), std::string("hello"), pred::is_equal{}, true);
    }

    {
        runner.test_evaluator("is_not_equal: different values", 5, 3, pred::is_not_equal{}, true);
        runner.test_evaluator("is_not_equal: same values", 5, 5, pred::is_not_equal{}, false);
    }

    {
        runner.test_evaluator("is_greater: 10 > 5", 10, 5, pred::is_greater{}, true);
        runner.test_evaluator("is_greater: 5 > 10", 5, 10, pred::is_greater{}, false);
        runner.test_evaluator("is_greater: equal values", 5, 5, pred::is_greater{}, false);
    }

    {
        runner.test_evaluator("is_less: 3 < 7", 3, 7, pred::is_less{}, true);
        runner.test_evaluator("is_less: 7 < 3", 7, 3, pred::is_less{}, false);
        runner.test_evaluator("is_less: equal values", 5, 5, pred::is_less{}, false);
    }

    {
        runner.test_evaluator("is_greater_equal: 10 >= 5", 10, 5, pred::is_greater_equal{}, true);
        runner.test_evaluator("is_greater_equal: 5 >= 5", 5, 5, pred::is_greater_equal{}, true);
        runner.test_evaluator("is_greater_equal: 3 >= 7", 3, 7, pred::is_greater_equal{}, false);
    }

    {
        runner.test_evaluator("is_less_equal: 3 <= 7", 3, 7, pred::is_less_equal{}, true);
        runner.test_evaluator("is_less_equal: 5 <= 5", 5, 5, pred::is_less_equal{}, true);
        runner.test_evaluator("is_less_equal: 10 <= 5", 10, 5, pred::is_less_equal{}, false);
    }

    // ==================== String Predicates ====================
    std::cout << "\n--- String Predicates ---" << std::endl;

    {
        runner.test_evaluator("contains_substring: basic", std::string("hello world"), std::string("world"), pred::contains_substring{}, true);
        runner.test_evaluator("contains_substring: not found", std::string("hello"), std::string("xyz"), pred::contains_substring{}, false);
        runner.test_evaluator("contains_substring: empty substring", std::string("hello"), std::string(""), pred::contains_substring{}, true);
        runner.test_evaluator("contains_substring: C-strings", "programming", "gram", pred::contains_substring{}, true);
        runner.test_evaluator("contains_substring: string_view", std::string_view("testing"), std::string_view("test"), pred::contains_substring{}, true);
    }

    {
        runner.test_evaluator("starts_with: basic", std::string("hello world"), std::string("hello"), pred::starts_with{}, true);
        runner.test_evaluator("starts_with: false case", std::string("hello world"), std::string("world"), pred::starts_with{}, false);
        runner.test_evaluator("starts_with: empty prefix", std::string("hello"), std::string(""), pred::starts_with{}, true);
        runner.test_evaluator("starts_with: same string", std::string("test"), std::string("test"), pred::starts_with{}, true);
        runner.test_evaluator("starts_with: longer prefix", std::string("hi"), std::string("hello"), pred::starts_with{}, false);
    }

    {
        runner.test_evaluator("ends_with: basic", std::string("hello world"), std::string("world"), pred::ends_with{}, true);
        runner.test_evaluator("ends_with: false case", std::string("hello world"), std::string("hello"), pred::ends_with{}, false);
        runner.test_evaluator("ends_with: empty suffix", std::string("hello"), std::string(""), pred::ends_with{}, true);
        runner.test_evaluator("ends_with: same string", std::string("test"), std::string("test"), pred::ends_with{}, true);
        runner.test_evaluator("ends_with: longer suffix", std::string("hi"), std::string("hello"), pred::ends_with{}, false);
    }

    // ==================== Range Predicates ====================
    std::cout << "\n--- Range Predicates ---" << std::endl;

    {
        tunit::Evaluator eval1(5, pred::is_in_range{}, [](const auto& val, const auto& pred) {
            return pred(val, 1, 10);
        });
        runner.test("is_in_range: value in range", pred::is_in_range{}(5, 1, 10));
        runner.test("is_in_range: value at lower bound", pred::is_in_range{}(1, 1, 10));
        runner.test("is_in_range: value at upper bound", pred::is_in_range{}(10, 1, 10));
        runner.test("is_in_range: value below range", !pred::is_in_range{}(0, 1, 10));
        runner.test("is_in_range: value above range", !pred::is_in_range{}(11, 1, 10));
    }

    {
        runner.test("is_out_of_range: value below range", pred::is_out_of_range{}(0, 1, 10));
        runner.test("is_out_of_range: value above range", pred::is_out_of_range{}(11, 1, 10));
        runner.test("is_out_of_range: value in range", !pred::is_out_of_range{}(5, 1, 10));
    }

    // ==================== Numeric Predicates ====================
    std::cout << "\n--- Numeric Predicates ---" << std::endl;

    {
        runner.test("is_even: even number", pred::is_even{}(4));
        runner.test("is_even: odd number", !pred::is_even{}(5));
        runner.test("is_even: zero", pred::is_even{}(0));
        runner.test("is_even: negative even", pred::is_even{}(-2));
        runner.test("is_even: negative odd", !pred::is_even{}(-3));
        
        // Test binary version
        runner.test("is_even: both even", pred::is_even{}(4, 6));
        runner.test("is_even: one odd", !pred::is_even{}(4, 5));
        runner.test("is_even: both odd", !pred::is_even{}(3, 5));
    }

    {
        runner.test("is_odd: odd number", pred::is_odd{}(5));
        runner.test("is_odd: even number", !pred::is_odd{}(4));
        runner.test("is_odd: zero", !pred::is_odd{}(0));
        runner.test("is_odd: negative odd", pred::is_odd{}(-3));
        runner.test("is_odd: negative even", !pred::is_odd{}(-2));
        
        // Test binary version
        runner.test("is_odd: both odd", pred::is_odd{}(3, 5));
        runner.test("is_odd: one even", !pred::is_odd{}(3, 4));
        runner.test("is_odd: both even", !pred::is_odd{}(2, 4));
    }

    {
        runner.test("is_positive: positive integer", pred::is_positive{}(5));
        runner.test("is_positive: negative integer", !pred::is_positive{}(-5));
        runner.test("is_positive: zero", !pred::is_positive{}(0));
        runner.test("is_positive: positive double", pred::is_positive{}(3.14));
        runner.test("is_positive: negative double", !pred::is_positive{}(-2.71));
        
        // Test binary version
        runner.test("is_positive: both positive", pred::is_positive{}(5, 3));
        runner.test("is_positive: one negative", !pred::is_positive{}(5, -3));
        runner.test("is_positive: both negative", !pred::is_positive{}(-5, -3));
    }

    {
        runner.test("is_negative: negative integer", pred::is_negative{}(-5));
        runner.test("is_negative: positive integer", !pred::is_negative{}(5));
        runner.test("is_negative: zero", !pred::is_negative{}(0));
        runner.test("is_negative: negative double", pred::is_negative{}(-3.14));
        runner.test("is_negative: positive double", !pred::is_negative{}(2.71));
        
        // Test binary version
        runner.test("is_negative: both negative", pred::is_negative{}(-5, -3));
        runner.test("is_negative: one positive", !pred::is_negative{}(-5, 3));
        runner.test("is_negative: both positive", !pred::is_negative{}(5, 3));
    }

    {
        runner.test("is_zero: zero integer", pred::is_zero{}(0));
        runner.test("is_zero: non-zero integer", !pred::is_zero{}(5));
        runner.test("is_zero: zero double", pred::is_zero{}(0.0));
        runner.test("is_zero: non-zero double", !pred::is_zero{}(3.14));
        
        // Test binary version
        runner.test("is_zero: both zero", pred::is_zero{}(0, 0));
        runner.test("is_zero: one non-zero", !pred::is_zero{}(0, 5));
        runner.test("is_zero: both non-zero", !pred::is_zero{}(3, 5));
    }

    // ==================== Edge Cases and Type Tests ====================
    std::cout << "\n--- Edge Cases ---" << std::endl;

    {
        // Test with different numeric types
        runner.test_evaluator("Mixed types: int/double", 5, 5.0, pred::is_equal{}, true);
        runner.test_evaluator("Mixed types: char/int", 'A', 65, pred::is_equal{}, true);
        runner.test_evaluator("Mixed types: short/long", short(100), 100L, pred::is_equal{}, true);
    }

    {
        // Test with extreme values
        runner.test("Large numbers", pred::is_greater{}(1000000, 999999));
        runner.test("Very negative", pred::is_less{}(-1000000, -999999));
        runner.test("Zero comparisons", pred::is_greater_equal{}(0, -1));
    }

    {
        // Performance benchmarks
        runner.benchmark("is_equal performance", []() {
            return pred::is_equal{}(42, 42);
        }, 100000);

        runner.benchmark("is_greater performance", []() {
            return pred::is_greater{}(100, 50);
        }, 100000);

        runner.benchmark("string contains performance", []() {
            return pred::contains_substring{}("this is a longer string for testing", "string");
        }, 10000);
    }

    runner.summary();
    return runner.all_passed() ? 0 : 1;
}
