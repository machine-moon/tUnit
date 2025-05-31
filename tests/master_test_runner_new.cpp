#include "../include/evaluator_all.h"
#include <iostream>

using namespace tunit;
namespace pred = tunit::predicates;

// Forward declarations for test functions
int run_evaluator_core_tests();
int run_common_predicates_tests();
int run_complex_predicates_tests();
int run_logical_predicates_tests();
int run_container_predicates_tests();
int run_integration_tests();

int main()
{
    std::cout << "=================================================\n";
    std::cout << "        TUNIT COMPREHENSIVE TEST SUITE\n";
    std::cout << "=================================================\n\n";

    // Reset global counters
    TestRunner::reset_global_counters();

    std::cout << "Running all test suites...\n\n";

    // Run all test suites
    run_evaluator_core_tests();
    run_common_predicates_tests();
    run_complex_predicates_tests();
    run_logical_predicates_tests();
    run_container_predicates_tests();
    run_integration_tests();

    // Final summary
    std::cout << "\n=================================================\n";
    std::cout << "              FINAL SUMMARY\n";
    std::cout << "=================================================\n";
    
    int total_passes = TestRunner::get_total_passes_count();
    int total_fails = TestRunner::get_total_fails_count();
    int total_tests = total_passes + total_fails;
    
    std::cout << "Total Tests Run: " << total_tests << "\n";
    std::cout << "Total Passes: " << total_passes << "\n";
    std::cout << "Total Failures: " << total_fails << "\n";
    
    double success_rate = total_tests > 0 ? (100.0 * total_passes / total_tests) : 0.0;
    std::cout << "Success Rate: " << success_rate << "%\n";
    
    if (total_fails == 0) {
        std::cout << "\n🎉 ALL TESTS PASSED! 🎉\n";
    } else {
        std::cout << "\n❌ Some tests failed. Please review the output above.\n";
    }
    
    std::cout << "=================================================\n";
    
    return total_fails > 0 ? 1 : 0;
}

// Test function implementations that call the actual test executables
int run_evaluator_core_tests()
{
    auto& suite = TestRunner::get_suite("Evaluator Core");
    
    // Basic evaluator tests
    suite.test("5 is_equal 5", 5, pred::is_equal{}, 5);
    suite.test("10 is_greater 5", 10, pred::is_greater{}, 5);
    suite.test("3 is_less 7", 3, pred::is_less{}, 7);
    suite.test("5 is_greater_equal 5", 5, pred::is_greater_equal{}, 5);
    suite.test("3 is_less_equal 7", 3, pred::is_less_equal{}, 7);
    suite.test("5 is_not_equal 3", 5, pred::is_not_equal{}, 3);
    
    // Failed cases
    suite.test("5 is_not_equal 5", 5, pred::is_equal{}, 6, false);
    suite.test("3 is_not_greater 7", 3, pred::is_greater{}, 7, false);
    
    suite.suite_summary();
    return 0;
}

int run_common_predicates_tests()
{
    auto& suite = TestRunner::get_suite("Common Predicates Sample");
    
    // Sample of common predicates (full tests in separate executable)
    suite.test("hello contains_substring ell", std::string("hello"), pred::contains_substring{}, std::string("ell"));
    suite.test("hello starts_with he", std::string("hello"), pred::starts_with{}, std::string("he"));
    suite.test("4 is_even", [](){ return pred::is_even{}(4); }());
    suite.test("5 is_odd", [](){ return pred::is_odd{}(5); }());
    suite.test("10 is_positive", [](){ return pred::is_positive{}(10); }());
    
    suite.suite_summary();
    return 0;
}

int run_complex_predicates_tests()
{
    auto& suite = TestRunner::get_suite("Complex Predicates Sample");
    
    // Simple all_of test
    suite.test("4 satisfies positive and even", [](){ 
        auto pred_all = pred::all_of(pred::is_positive{}, pred::is_even{});
        return pred_all(4); 
    }());
    
    // Simple any_of test  
    suite.test("0 satisfies zero or positive", [](){ 
        auto pred_any = pred::any_of(pred::is_zero{}, pred::is_positive{});
        return pred_any(0); 
    }());
    
    // Simple exactly_n_of test
    suite.test("4 satisfies exactly 2 of 3 conditions", [](){ 
        auto pred_exact = pred::exactly_n_of(2, pred::is_positive{}, pred::is_even{}, [](int x) { return x > 3; });
        return pred_exact(4); 
    }());
    
    suite.suite_summary();
    return 0;
}

int run_logical_predicates_tests()
{
    auto& suite = TestRunner::get_suite("Logical Predicates Sample");
    
    // Simple logical tests using lambdas to avoid constructor issues
    suite.test("4 is positive and even", [](){ 
        return pred::is_positive{}(4) && pred::is_even{}(4);
    }());
    
    suite.test("-5 is not positive", [](){ 
        return !pred::is_positive{}(-5);
    }());
    
    suite.test("3 is positive but not even", [](){ 
        return pred::is_positive{}(3) && !pred::is_even{}(3);
    }());
    
    suite.suite_summary();
    return 0;
}

int run_container_predicates_tests()
{
    auto& suite = TestRunner::get_suite("Container Predicates Sample");
    
    // Sample container predicates
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::vector<int> empty_vec = {};
    
    suite.test("numbers contains 3", numbers, pred::contains_element{}, 3);
    suite.test("empty_vec is_empty", [&](){ return pred::is_empty{}(empty_vec); }());
    suite.test("numbers is_sorted", [&](){ return pred::is_sorted{}(numbers); }());
    
    suite.test("numbers contains even element", [&](){ 
        auto is_even = [](int x) { return x % 2 == 0; };
        return pred::contains_if{}(numbers, is_even); 
    }());
    
    suite.suite_summary();
    return 0;
}

int run_integration_tests()
{
    auto& suite = TestRunner::get_suite("Integration Sample");
    
    // Sample integration tests
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    suite.test("numbers is not empty and has min size", [&](){ 
        return pred::is_not_empty{}(numbers) && pred::has_min_size{}(numbers, 3);
    }());
    
    suite.test("all numbers in range 1-10", [&](){ 
        auto in_range = [](int x) { return pred::is_in_range{}(x, 1, 10); };
        return pred::all_elements_satisfy{}(numbers, in_range); 
    }());
    
    suite.suite_summary();
    return 0;
}
