#include "../include/evaluator_all.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

namespace pred = tunit::predicates;

// Test suite information
struct TestSuite {
    std::string name;
    std::string description;
    std::function<int()> run_function;
};

// Forward declarations of test functions
int run_evaluator_core_tests();
int run_common_predicates_tests();
int run_complex_predicates_tests();
int run_logical_predicates_tests();
int run_container_predicates_tests();
int run_integration_tests();

int main(int argc, char* argv[])
{
    tunit::TestRunner master_runner("TUnit Master Test Suite");
    
    std::cout << std::string(70, '=') << std::endl;
    std::cout << "        TUnit Testing Framework - Complete Test Suite" << std::endl;
    std::cout << std::string(70, '=') << std::endl;

    // Define all test suites
    std::vector<TestSuite> test_suites = {
        {
            "Evaluator Core",
            "Tests core Evaluator class functionality and API",
            []() {
                std::cout << "\n" << std::string(50, '-') << std::endl;
                std::cout << "Running Evaluator Core Tests..." << std::endl;
                std::cout << std::string(50, '-') << std::endl;
                // Simulate running the test (in real scenario, these would be actual test calls)
                tunit::TestRunner runner("Evaluator Core");
                
                // Basic functionality tests
                runner.test_evaluator("Basic equal", 5, 5, pred::is_equal{}, true);
                runner.test_evaluator("Basic greater", 10, 5, pred::is_greater{}, true);
                runner.test_evaluator("Basic less", 3, 7, pred::is_less{}, true);
                
                // API method tests
                tunit::Evaluator eval(10, 5, pred::is_greater{});
                runner.test("operator() method", eval());
                runner.test("evaluate() method", eval.evaluate());
                runner.test("use() method", eval.use(20, 15));
                runner.test("compare() method", eval.compare(3));
                
                runner.summary();
                return runner.all_passed() ? 0 : 1;
            }
        },
        {
            "Common Predicates",
            "Tests basic comparison, string, range, and numeric predicates",
            []() {
                std::cout << "\n" << std::string(50, '-') << std::endl;
                std::cout << "Running Common Predicates Tests..." << std::endl;
                std::cout << std::string(50, '-') << std::endl;
                
                tunit::TestRunner runner("Common Predicates");
                
                // Comparison predicates
                runner.test_evaluator("is_equal", 5, 5, pred::is_equal{}, true);
                runner.test_evaluator("is_not_equal", 5, 3, pred::is_not_equal{}, true);
                runner.test_evaluator("is_greater", 10, 5, pred::is_greater{}, true);
                runner.test_evaluator("is_less", 3, 7, pred::is_less{}, true);
                runner.test_evaluator("is_greater_equal", 5, 5, pred::is_greater_equal{}, true);
                runner.test_evaluator("is_less_equal", 3, 7, pred::is_less_equal{}, true);
                
                // String predicates
                runner.test("contains_substring", pred::contains_substring{}("hello world", "world"));
                runner.test("starts_with", pred::starts_with{}("hello world", "hello"));
                runner.test("ends_with", pred::ends_with{}("hello world", "world"));
                
                // Range predicates
                runner.test("is_in_range", pred::is_in_range{}(5, 1, 10));
                runner.test("is_out_of_range", pred::is_out_of_range{}(15, 1, 10));
                
                // Numeric predicates
                runner.test("is_even", pred::is_even{}(4));
                runner.test("is_odd", pred::is_odd{}(5));
                runner.test("is_positive", pred::is_positive{}(10));
                runner.test("is_negative", pred::is_negative{}(-5));
                runner.test("is_zero", pred::is_zero{}(0));
                
                runner.summary();
                return runner.all_passed() ? 0 : 1;
            }
        },
        {
            "Complex Predicates", 
            "Tests advanced predicate combinations and logical constructs",
            []() {
                std::cout << "\n" << std::string(50, '-') << std::endl;
                std::cout << "Running Complex Predicates Tests..." << std::endl;
                std::cout << std::string(50, '-') << std::endl;
                
                tunit::TestRunner runner("Complex Predicates");
                
                // all_of predicate
                auto all_positive_even = pred::all_of{pred::is_positive{}, pred::is_even{}};
                runner.test("all_of: positive and even", all_positive_even(4));
                runner.test("all_of: positive but odd", !all_positive_even(3));
                
                // any_of predicate
                auto any_special = pred::any_of{pred::is_zero{}, pred::is_negative{}};
                runner.test("any_of: zero case", any_special(0));
                runner.test("any_of: negative case", any_special(-5));
                runner.test("any_of: positive case", !any_special(5));
                
                // none_of predicate
                auto none_bad = pred::none_of{pred::is_negative{}, pred::is_zero{}};
                runner.test("none_of: positive", none_bad(5));
                runner.test("none_of: negative", !none_bad(-5));
                
                // conditional predicate
                auto conditional_check = pred::conditional{
                    pred::is_positive{},
                    pred::is_even{},
                    pred::is_odd{}
                };
                runner.test("conditional: positive even", conditional_check(4));
                runner.test("conditional: negative odd", conditional_check(-3));
                
                // counting predicates
                auto exactly_one = pred::exactly_n_of{1, pred::is_positive{}, pred::is_negative{}, pred::is_zero{}};
                runner.test("exactly_n_of: positive", exactly_one(5));
                runner.test("exactly_n_of: zero", exactly_one(0));
                
                auto at_least_one = pred::at_least_n_of{1, pred::is_even{}, pred::is_odd{}};
                runner.test("at_least_n_of: even", at_least_one(4));
                runner.test("at_least_n_of: odd", at_least_one(3));
                
                auto at_most_zero = pred::at_most_n_of{0, pred::is_negative{}, pred::is_zero{}};
                runner.test("at_most_n_of: positive", at_most_zero(5));
                
                runner.summary();
                return runner.all_passed() ? 0 : 1;
            }
        },
        {
            "Logical Predicates",
            "Tests logical operators and boolean algebra predicates",
            []() {
                std::cout << "\n" << std::string(50, '-') << std::endl;
                std::cout << "Running Logical Predicates Tests..." << std::endl;
                std::cout << std::string(50, '-') << std::endl;
                
                tunit::TestRunner runner("Logical Predicates");
                
                // Basic logical operations
                auto and_pred = pred::and_{pred::is_positive{}, pred::is_even{}};
                runner.test("and_: positive and even", and_pred(4));
                runner.test("and_: positive but odd", !and_pred(3));
                
                auto or_pred = pred::or_{pred::is_negative{}, pred::is_even{}};
                runner.test("or_: negative", or_pred(-3));
                runner.test("or_: even", or_pred(4));
                runner.test("or_: positive odd", !or_pred(3));
                
                auto xor_pred = pred::xor_{pred::is_positive{}, pred::is_even{}};
                runner.test("xor_: positive odd", xor_pred(3));
                runner.test("xor_: negative even", xor_pred(-4));
                runner.test("xor_: positive even", !xor_pred(4));
                
                auto not_pred = pred::not_{pred::is_positive{}};
                runner.test("not_: negative", not_pred(-5));
                runner.test("not_: positive", !not_pred(5));
                
                // Advanced logical operations
                auto nand_pred = pred::nand_{pred::is_positive{}, pred::is_even{}};
                runner.test("nand_: not (positive and even)", !nand_pred(4));
                runner.test("nand_: positive odd", nand_pred(3));
                
                auto nor_pred = pred::nor_{pred::is_negative{}, pred::is_zero{}};
                runner.test("nor_: positive", nor_pred(5));
                runner.test("nor_: negative", !nor_pred(-5));
                
                auto implies_pred = pred::implies{pred::is_positive{}, pred::is_even{}};
                runner.test("implies: positive even", implies_pred(4));
                runner.test("implies: negative even", implies_pred(-4));
                runner.test("implies: positive odd", !implies_pred(3));
                
                runner.summary();
                return runner.all_passed() ? 0 : 1;
            }
        },
        {
            "Container Predicates",
            "Tests container operations and algorithms",
            []() {
                std::cout << "\n" << std::string(50, '-') << std::endl;
                std::cout << "Running Container Predicates Tests..." << std::endl;
                std::cout << std::string(50, '-') << std::endl;
                
                tunit::TestRunner runner("Container Predicates");
                
                std::vector<int> vec = {1, 2, 3, 4, 5};
                std::vector<int> empty_vec;
                std::vector<int> sorted_vec = {1, 2, 3, 4, 5};
                std::vector<int> unsorted_vec = {3, 1, 4, 1, 5};
                
                // Element presence
                runner.test("contains_element", pred::contains_element{}(vec, 3));
                runner.test("contains_element false", !pred::contains_element{}(vec, 10));
                
                auto is_even = [](int x) { return x % 2 == 0; };
                runner.test("contains_if", pred::contains_if{}(vec, is_even));
                
                std::vector<int> subset = {1, 3, 5};
                runner.test("contains_all_elements", pred::contains_all_elements{}(vec, subset));
                runner.test("contains_any_elements", pred::contains_any_elements{}(vec, subset));
                
                // Size predicates
                runner.test("is_empty", pred::is_empty{}(empty_vec));
                runner.test("is_not_empty", pred::is_not_empty{}(vec));
                runner.test("has_size", pred::has_size{}(vec, 5));
                runner.test("has_min_size", pred::has_min_size{}(vec, 3));
                runner.test("has_max_size", pred::has_max_size{}(vec, 7));
                
                // Algorithm predicates
                runner.test("all_elements_satisfy", 
                           pred::all_elements_satisfy{}(vec, [](int x) { return x > 0; }));
                runner.test("any_element_satisfies", pred::any_element_satisfies{}(vec, is_even));
                runner.test("no_element_satisfies", 
                           pred::no_element_satisfies{}(vec, [](int x) { return x < 0; }));
                
                // Ordering predicates
                runner.test("is_sorted", pred::is_sorted{}(sorted_vec));
                runner.test("is_sorted false", !pred::is_sorted{}(unsorted_vec));
                runner.test("is_unique", pred::is_unique{}(vec));
                
                // Equality predicates
                std::vector<int> vec_copy = {1, 2, 3, 4, 5};
                runner.test("containers_equal", pred::containers_equal{}(vec, vec_copy));
                
                std::vector<int> permutation = {5, 4, 3, 2, 1};
                runner.test("is_permutation_of", pred::is_permutation_of{}(vec, permutation));
                
                runner.summary();
                return runner.all_passed() ? 0 : 1;
            }
        },
        {
            "Integration Tests",
            "Tests real-world scenarios and complex combinations",
            []() {
                std::cout << "\n" << std::string(50, '-') << std::endl;
                std::cout << "Running Integration Tests..." << std::endl;
                std::cout << std::string(50, '-') << std::endl;
                
                tunit::TestRunner runner("Integration Tests");
                
                // Real-world validation scenario
                struct User {
                    std::string name;
                    int age;
                    bool is_admin;
                };
                
                User valid_user{"John Doe", 25, false};
                User admin_user{"Admin", 30, true};
                User young_user{"Kid", 15, false};
                
                auto is_valid_user = pred::all_of{
                    [](const User& u) { return !u.name.empty(); },
                    pred::or_{
                        [](const User& u) { return u.is_admin; },
                        [](const User& u) { return u.age >= 18; }
                    }
                };
                
                runner.test("Integration: valid adult user", is_valid_user(valid_user));
                runner.test("Integration: admin user", is_valid_user(admin_user));
                runner.test("Integration: young user", !is_valid_user(young_user));
                
                // Complex data validation
                std::vector<int> data = {2, 4, 6, 8, 10};
                auto valid_data = pred::all_of{
                    pred::is_not_empty{},
                    pred::is_sorted{},
                    [](const std::vector<int>& v) { 
                        return pred::all_elements_satisfy{}(v, pred::is_even{}); 
                    }
                };
                
                runner.test("Integration: valid data set", valid_data(data));
                
                // Performance test
                std::vector<int> large_vec(10000);
                std::iota(large_vec.begin(), large_vec.end(), 1);
                
                auto start = std::chrono::high_resolution_clock::now();
                bool result = pred::contains_element{}(large_vec, 5000);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                
                runner.test("Integration: large dataset performance", result);
                std::cout << "  Performance: " << duration.count() << "μs for 10k elements\n";
                
                runner.summary();
                return runner.all_passed() ? 0 : 1;
            }
        }
    };

    // Run all test suites
    std::vector<bool> results;
    int total_tests = 0;
    int passed_tests = 0;
    
    auto overall_start = std::chrono::high_resolution_clock::now();
    
    for (const auto& suite : test_suites) {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "Test Suite: " << suite.name << std::endl;
        std::cout << "Description: " << suite.description << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        auto suite_start = std::chrono::high_resolution_clock::now();
        int result = suite.run_function();
        auto suite_end = std::chrono::high_resolution_clock::now();
        auto suite_duration = std::chrono::duration_cast<std::chrono::milliseconds>(suite_end - suite_start);
        
        results.push_back(result == 0);
        if (result == 0) {
            std::cout << "✓ " << suite.name << " PASSED";
            passed_tests++;
        } else {
            std::cout << "✗ " << suite.name << " FAILED";
        }
        std::cout << " (Duration: " << suite_duration.count() << "ms)" << std::endl;
        total_tests++;
    }
    
    auto overall_end = std::chrono::high_resolution_clock::now();
    auto overall_duration = std::chrono::duration_cast<std::chrono::milliseconds>(overall_end - overall_start);
    
    // Final summary
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "                    FINAL TEST SUMMARY" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    std::cout << "Test Suites Run: " << total_tests << std::endl;
    std::cout << "Passed: " << passed_tests << std::endl;
    std::cout << "Failed: " << (total_tests - passed_tests) << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(1) 
              << (static_cast<double>(passed_tests) / total_tests * 100.0) << "%" << std::endl;
    std::cout << "Total Duration: " << overall_duration.count() << "ms" << std::endl;
    
    if (passed_tests == total_tests) {
        std::cout << "\n🎉 ALL TESTS PASSED! 🎉" << std::endl;
        std::cout << "The TUnit library is working correctly!" << std::endl;
    } else {
        std::cout << "\n❌ SOME TESTS FAILED" << std::endl;
        std::cout << "Failed test suites:" << std::endl;
        for (size_t i = 0; i < results.size(); ++i) {
            if (!results[i]) {
                std::cout << "  - " << test_suites[i].name << std::endl;
            }
        }
    }
    
    std::cout << std::string(70, '=') << std::endl;
    
    return (passed_tests == total_tests) ? 0 : 1;
}
