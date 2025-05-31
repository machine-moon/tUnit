#include "../include/evaluator_all.h"
#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <deque>
#include <set>
#include <string>
#include <algorithm>

namespace pred = tunit::predicates;

int main()
{
    tunit::TestRunner runner("Container Predicates Tests");

    std::cout << "=== Testing Container Predicates ===" << std::endl;

    // ==================== Element Presence Predicates ====================
    std::cout << "\n--- Element Presence Predicates ---" << std::endl;

    {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::array<int, 5> arr = {1, 2, 3, 4, 5};
        std::list<int> lst = {1, 2, 3, 4, 5};
        
        runner.test("contains_element: vector contains", pred::contains_element{}(vec, 3));
        runner.test("contains_element: vector doesn't contain", !pred::contains_element{}(vec, 10));
        runner.test("contains_element: array contains", pred::contains_element{}(arr, 4));
        runner.test("contains_element: list contains", pred::contains_element{}(lst, 1));
        
        // Test with different types
        std::vector<std::string> str_vec = {"hello", "world", "test"};
        runner.test("contains_element: string vector", pred::contains_element{}(str_vec, std::string("world")));
        runner.test("contains_element: string vector negative", !pred::contains_element{}(str_vec, std::string("missing")));
    }

    {
        std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        
        auto is_even = [](int x) { return x % 2 == 0; };
        auto is_greater_than_5 = [](int x) { return x > 5; };
        auto is_negative = [](int x) { return x < 0; };
        
        runner.test("contains_if: contains even", pred::contains_if{}(vec, is_even));
        runner.test("contains_if: contains > 5", pred::contains_if{}(vec, is_greater_than_5));
        runner.test("contains_if: no negative", !pred::contains_if{}(vec, is_negative));
    }

    {
        std::vector<int> container = {1, 2, 3, 4, 5};
        std::vector<int> all_present = {1, 3, 5};
        std::vector<int> some_missing = {1, 3, 6};
        std::vector<int> none_present = {10, 11, 12};
        
        runner.test("contains_all_elements: all present", pred::contains_all_elements{}(container, all_present));
        runner.test("contains_all_elements: some missing", !pred::contains_all_elements{}(container, some_missing));
        runner.test("contains_all_elements: none present", !pred::contains_all_elements{}(container, none_present));
        
        // Edge case: empty search set
        std::vector<int> empty_search;
        runner.test("contains_all_elements: empty search", pred::contains_all_elements{}(container, empty_search));
    }

    {
        std::vector<int> container = {1, 2, 3, 4, 5};
        std::vector<int> some_present = {1, 6, 7};
        std::vector<int> none_present = {10, 11, 12};
        std::vector<int> all_present = {1, 2, 3};
        
        runner.test("contains_any_elements: some present", pred::contains_any_elements{}(container, some_present));
        runner.test("contains_any_elements: all present", pred::contains_any_elements{}(container, all_present));
        runner.test("contains_any_elements: none present", !pred::contains_any_elements{}(container, none_present));
    }

    // ==================== Size Predicates ====================
    std::cout << "\n--- Size Predicates ---" << std::endl;

    {
        std::vector<int> empty_vec;
        std::vector<int> non_empty = {1, 2, 3};
        std::string empty_str = "";
        std::string non_empty_str = "hello";
        
        runner.test("is_empty: empty vector", pred::is_empty{}(empty_vec));
        runner.test("is_empty: non-empty vector", !pred::is_empty{}(non_empty));
        runner.test("is_empty: empty string", pred::is_empty{}(empty_str));
        runner.test("is_empty: non-empty string", !pred::is_empty{}(non_empty_str));
    }

    {
        std::vector<int> empty_vec;
        std::vector<int> non_empty = {1, 2, 3};
        
        runner.test("is_not_empty: non-empty vector", pred::is_not_empty{}(non_empty));
        runner.test("is_not_empty: empty vector", !pred::is_not_empty{}(empty_vec));
    }

    {
        std::vector<int> vec3 = {1, 2, 3};
        std::vector<int> vec5 = {1, 2, 3, 4, 5};
        std::string str4 = "test";
        
        runner.test("has_size: correct size vector", pred::has_size{}(vec3, 3));
        runner.test("has_size: wrong size vector", !pred::has_size{}(vec5, 3));
        runner.test("has_size: string size", pred::has_size{}(str4, 4));
        runner.test("has_size: zero size", pred::has_size{}(std::vector<int>{}, 0));
    }

    {
        std::vector<int> vec5 = {1, 2, 3, 4, 5};
        
        runner.test("has_min_size: meets minimum", pred::has_min_size{}(vec5, 3));
        runner.test("has_min_size: equals minimum", pred::has_min_size{}(vec5, 5));
        runner.test("has_min_size: below minimum", !pred::has_min_size{}(vec5, 7));
        runner.test("has_min_size: zero minimum", pred::has_min_size{}(vec5, 0));
    }

    {
        std::vector<int> vec5 = {1, 2, 3, 4, 5};
        
        runner.test("has_max_size: below maximum", pred::has_max_size{}(vec5, 7));
        runner.test("has_max_size: equals maximum", pred::has_max_size{}(vec5, 5));
        runner.test("has_max_size: above maximum", !pred::has_max_size{}(vec5, 3));
    }

    // ==================== Algorithm-based Predicates ====================
    std::cout << "\n--- Algorithm-based Predicates ---" << std::endl;

    {
        std::vector<int> has_evens = {1, 2, 3, 4, 5};
        std::vector<int> all_odds = {1, 3, 5, 7, 9};
        
        auto is_even = [](int x) { return x % 2 == 0; };
        
        runner.test("all_elements_satisfy: not all even", !pred::all_elements_satisfy{}(has_evens, is_even));
        runner.test("all_elements_satisfy: all odd (none even)", pred::all_elements_satisfy{}(all_odds, [](int x) { return x % 2 == 1; }));
        
        // Edge case: empty container
        std::vector<int> empty_vec;
        runner.test("all_elements_satisfy: empty container", pred::all_elements_satisfy{}(empty_vec, is_even));
    }

    {
        std::vector<int> has_evens = {1, 2, 3, 4, 5};
        std::vector<int> all_odds = {1, 3, 5, 7, 9};
        
        auto is_even = [](int x) { return x % 2 == 0; };
        
        runner.test("any_element_satisfies: has even", pred::any_element_satisfies{}(has_evens, is_even));
        runner.test("any_element_satisfies: no even", !pred::any_element_satisfies{}(all_odds, is_even));
        
        // Edge case: empty container
        std::vector<int> empty_vec;
        runner.test("any_element_satisfies: empty container", !pred::any_element_satisfies{}(empty_vec, is_even));
    }

    {
        std::vector<int> has_evens = {1, 2, 3, 4, 5};
        std::vector<int> all_odds = {1, 3, 5, 7, 9};
        
        auto is_even = [](int x) { return x % 2 == 0; };
        
        runner.test("no_element_satisfies: has even", !pred::no_element_satisfies{}(has_evens, is_even));
        runner.test("no_element_satisfies: no even", pred::no_element_satisfies{}(all_odds, is_even));
    }

    {
        std::vector<int> vec = {2, 4, 6, 8, 10}; // 5 even numbers
        auto is_even = [](int x) { return x % 2 == 0; };
        
        runner.test("exactly_n_elements_satisfy: exactly 5", pred::exactly_n_elements_satisfy{}(vec, 5, is_even));
        runner.test("exactly_n_elements_satisfy: not 3", !pred::exactly_n_elements_satisfy{}(vec, 3, is_even));
        runner.test("exactly_n_elements_satisfy: not 7", !pred::exactly_n_elements_satisfy{}(vec, 7, is_even));
        
        std::vector<int> mixed = {1, 2, 3, 4}; // 2 even numbers
        runner.test("exactly_n_elements_satisfy: mixed container", pred::exactly_n_elements_satisfy{}(mixed, 2, is_even));
    }

    {
        std::vector<int> vec = {2, 4, 6, 8, 10}; // 5 even numbers
        auto is_even = [](int x) { return x % 2 == 0; };
        
        runner.test("at_least_n_elements_satisfy: at least 3", pred::at_least_n_elements_satisfy{}(vec, 3, is_even));
        runner.test("at_least_n_elements_satisfy: at least 5", pred::at_least_n_elements_satisfy{}(vec, 5, is_even));
        runner.test("at_least_n_elements_satisfy: not at least 6", !pred::at_least_n_elements_satisfy{}(vec, 6, is_even));
    }

    {
        std::vector<int> vec = {2, 4, 6, 8, 10}; // 5 even numbers
        auto is_even = [](int x) { return x % 2 == 0; };
        
        runner.test("at_most_n_elements_satisfy: at most 7", pred::at_most_n_elements_satisfy{}(vec, 7, is_even));
        runner.test("at_most_n_elements_satisfy: at most 5", pred::at_most_n_elements_satisfy{}(vec, 5, is_even));
        runner.test("at_most_n_elements_satisfy: not at most 3", !pred::at_most_n_elements_satisfy{}(vec, 3, is_even));
    }

    // ==================== Ordering Predicates ====================
    std::cout << "\n--- Ordering Predicates ---" << std::endl;

    {
        std::vector<int> sorted_asc = {1, 2, 3, 4, 5};
        std::vector<int> sorted_desc = {5, 4, 3, 2, 1};
        std::vector<int> unsorted = {3, 1, 4, 1, 5};
        std::vector<int> equal_elements = {2, 2, 2, 2};
        std::vector<int> empty_vec;
        std::vector<int> single = {42};
        
        runner.test("is_sorted: ascending", pred::is_sorted{}(sorted_asc));
        runner.test("is_sorted: descending (not sorted asc)", !pred::is_sorted{}(sorted_desc));
        runner.test("is_sorted: unsorted", !pred::is_sorted{}(unsorted));
        runner.test("is_sorted: equal elements", pred::is_sorted{}(equal_elements));
        runner.test("is_sorted: empty", pred::is_sorted{}(empty_vec));
        runner.test("is_sorted: single element", pred::is_sorted{}(single));
        
        // Test with custom comparator
        runner.test("is_sorted: descending with greater comparator", 
                   pred::is_sorted{}(sorted_desc, std::greater<int>{}));
    }

    {
        std::vector<int> sorted_desc = {5, 4, 3, 2, 1};
        std::vector<int> sorted_asc = {1, 2, 3, 4, 5};
        std::vector<int> unsorted = {3, 1, 4, 1, 5};
        
        runner.test("is_reverse_sorted: descending", pred::is_reverse_sorted{}(sorted_desc));
        runner.test("is_reverse_sorted: ascending", !pred::is_reverse_sorted{}(sorted_asc));
        runner.test("is_reverse_sorted: unsorted", !pred::is_reverse_sorted{}(unsorted));
    }

    {
        std::vector<int> unique_vec = {1, 2, 3, 4, 5};
        std::vector<int> duplicates = {1, 2, 2, 3, 4};
        std::vector<int> all_same = {2, 2, 2, 2};
        std::vector<int> empty_vec;
        std::vector<int> single = {42};
        
        runner.test("is_unique: unique elements", pred::is_unique{}(unique_vec));
        runner.test("is_unique: has duplicates", !pred::is_unique{}(duplicates));
        runner.test("is_unique: all same", !pred::is_unique{}(all_same));
        runner.test("is_unique: empty", pred::is_unique{}(empty_vec));
        runner.test("is_unique: single element", pred::is_unique{}(single));
    }

    // ==================== Equality Predicates ====================
    std::cout << "\n--- Equality Predicates ---" << std::endl;

    {
        std::vector<int> vec1 = {1, 2, 3, 4, 5};
        std::vector<int> vec2 = {1, 2, 3, 4, 5};
        std::vector<int> vec3 = {1, 2, 3, 4, 6};
        std::vector<int> vec4 = {1, 2, 3, 4};
        std::array<int, 5> arr = {1, 2, 3, 4, 5};
        
        runner.test("containers_equal: same vectors", pred::containers_equal{}(vec1, vec2));
        runner.test("containers_equal: different values", !pred::containers_equal{}(vec1, vec3));
        runner.test("containers_equal: different sizes", !pred::containers_equal{}(vec1, vec4));
        runner.test("containers_equal: vector and array", pred::containers_equal{}(vec1, arr));
        
        // Test with custom predicate
        auto abs_equal = [](int a, int b) { return std::abs(a) == std::abs(b); };
        std::vector<int> pos = {1, 2, 3};
        std::vector<int> neg = {-1, -2, -3};
        runner.test("containers_equal: custom predicate", pred::containers_equal{}(pos, neg, abs_equal));
    }

    {
        std::vector<int> vec1 = {1, 2, 3};
        std::vector<int> vec2 = {3, 1, 2}; // permutation
        std::vector<int> vec3 = {1, 2, 4}; // not permutation
        std::vector<int> vec4 = {1, 2, 2, 3}; // different size
        
        runner.test("is_permutation_of: same order", pred::is_permutation_of{}(vec1, vec1));
        runner.test("is_permutation_of: different order", pred::is_permutation_of{}(vec1, vec2));
        runner.test("is_permutation_of: different elements", !pred::is_permutation_of{}(vec1, vec3));
        runner.test("is_permutation_of: different sizes", !pred::is_permutation_of{}(vec1, vec4));
        
        // Test with duplicates
        std::vector<int> dup1 = {1, 1, 2, 3};
        std::vector<int> dup2 = {3, 1, 2, 1};
        runner.test("is_permutation_of: with duplicates", pred::is_permutation_of{}(dup1, dup2));
    }

    // ==================== Integration with Evaluator ====================
    std::cout << "\n--- Integration with Evaluator ---" << std::endl;

    {
        std::vector<int> vec1 = {1, 2, 3, 4, 5};
        std::vector<int> vec2 = {1, 2, 3, 4, 5};
        
        runner.test_evaluator("Evaluator with containers_equal", vec1, vec2, pred::containers_equal{}, true);
        
        tunit::Evaluator eval(vec1, 3, pred::contains_element{});
        runner.test("Evaluator contains element", eval());
        
        // Test compare method
        runner.test("Evaluator compare method", eval.compare(3));
        runner.test("Evaluator compare method false", !eval.compare(10));
    }

    // ==================== Different Container Types ====================
    std::cout << "\n--- Different Container Types ---" << std::endl;

    {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::list<int> lst = {1, 2, 3, 4, 5};
        std::deque<int> deq = {1, 2, 3, 4, 5};
        std::array<int, 5> arr = {1, 2, 3, 4, 5};
        
        runner.test("different types: vector/list equal", pred::containers_equal{}(vec, lst));
        runner.test("different types: vector/deque equal", pred::containers_equal{}(vec, deq));
        runner.test("different types: vector/array equal", pred::containers_equal{}(vec, arr));
        
        runner.test("different types: list contains", pred::contains_element{}(lst, 3));
        runner.test("different types: deque sorted", pred::is_sorted{}(deq));
    }

    // ==================== Performance Tests ====================
    std::cout << "\n--- Performance Tests ---" << std::endl;

    {
        std::vector<int> large_vec(10000);
        std::iota(large_vec.begin(), large_vec.end(), 1);
        
        runner.benchmark("contains_element large vector", [&]() {
            return pred::contains_element{}(large_vec, 5000);
        }, 1000);
        
        runner.benchmark("is_sorted large vector", [&]() {
            return pred::is_sorted{}(large_vec);
        }, 1000);
        
        std::vector<int> large_vec_copy = large_vec;
        runner.benchmark("containers_equal large vectors", [&]() {
            return pred::containers_equal{}(large_vec, large_vec_copy);
        }, 100);
    }

    // ==================== Edge Cases ====================
    std::cout << "\n--- Edge Cases ---" << std::endl;

    {
        // Empty containers
        std::vector<int> empty1, empty2;
        runner.test("empty containers equal", pred::containers_equal{}(empty1, empty2));
        runner.test("empty container sorted", pred::is_sorted{}(empty1));
        runner.test("empty container unique", pred::is_unique{}(empty1));
        
        // Single element containers
        std::vector<int> single1 = {42};
        std::vector<int> single2 = {42};
        runner.test("single element equal", pred::containers_equal{}(single1, single2));
        runner.test("single element contains", pred::contains_element{}(single1, 42));
    }

    runner.summary();
    return runner.all_passed() ? 0 : 1;
}
