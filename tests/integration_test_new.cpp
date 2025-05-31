#include "../include/evaluator_all.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace tunit;
namespace pred = tunit::predicates;

int main()
{
    auto& suite = TestRunner::get_suite("Integration Tests");

    // Test data
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<std::string> words = {"hello", "world", "test", "integration"};

    // Complex integration test: combining logical, complex, and container predicates
    auto complex_container_test = pred::and_{
        pred::all_of{
            [](const auto& vec) { return pred::is_not_empty{}(vec); },
            [](const auto& vec) { return pred::has_min_size{}(vec, 5); }
        },
        pred::any_of{
            [](const auto& vec) { return pred::all_elements_satisfy{}(vec, [](int x) { return x > 0; }); },
            [](const auto& vec) { return pred::contains_element{}(vec, 0); }
        }
    };
    
    suite.test("numbers satisfies complex container logic", [&](){ 
        return complex_container_test(numbers); 
    }());

    // Integration test: combining numeric predicates with container operations
    auto even_positive_test = pred::all_of{
        pred::is_positive{},
        pred::is_even{}
    };
    
    suite.test("numbers has elements satisfying even_and_positive", [&](){ 
        return pred::any_element_satisfies{}(numbers, [&](int x) { 
            return even_positive_test(x); 
        }); 
    }());

    // String and container integration
    auto long_word_test = [](const std::string& word) { 
        return pred::has_min_size{}(word, 5); 
    };
    
    suite.test("words contains long word", [&](){ 
        return pred::contains_if{}(words, long_word_test); 
    }());

    // Conditional logic with containers
    auto conditional_container = pred::conditional{
        [](const auto& vec) { return pred::is_empty{}(vec); },
        [](const auto& vec) { return true; },  // empty containers are valid
        [](const auto& vec) { return pred::is_sorted{}(vec); }  // non-empty must be sorted
    };
    
    suite.test("numbers satisfies conditional (empty->valid | non-empty->sorted)", [&](){ 
        return conditional_container(numbers); 
    }());

    std::vector<int> empty_vec = {};
    suite.test("empty_vec satisfies conditional", [&](){ 
        return conditional_container(empty_vec); 
    }());

    // Range predicates with container elements
    auto in_valid_range = [](int x) { 
        return pred::is_in_range{}(x, 1, 10); 
    };
    
    suite.test("all numbers are in valid range 1-10", [&](){ 
        return pred::all_elements_satisfy{}(numbers, in_valid_range); 
    }());

    // Complex logical combinations
    auto complex_element_predicate = pred::or_{
        pred::and_{pred::is_even{}, pred::is_greater{}},
        pred::and_{pred::is_odd{}, pred::is_less{}}
    };
    
    // Create test that uses two-argument predicates correctly
    auto test_complex_pairs = [&]() {
        for (size_t i = 0; i < numbers.size() - 1; ++i) {
            int a = numbers[i];
            int b = numbers[i + 1];
            if (pred::or_{
                pred::and_{pred::is_even{}, pred::is_greater{}}(a, b),
                pred::and_{pred::is_odd{}, pred::is_less{}}(a, b)
            }) {
                return true;
            }
        }
        return false;
    };
    
    suite.test("numbers contain pairs satisfying complex logic", test_complex_pairs());

    // String predicates with complex logic
    auto word_analysis = pred::all_of{
        [](const std::string& word) { return pred::has_min_size{}(word, 3); },
        [](const std::string& word) { return pred::starts_with{}(word, std::string("h")) || 
                                             pred::starts_with{}(word, std::string("w")) ||
                                             pred::starts_with{}(word, std::string("t")) ||
                                             pred::starts_with{}(word, std::string("i")); }
    };
    
    suite.test("all words satisfy analysis criteria", [&](){ 
        return pred::all_elements_satisfy{}(words, [&](const std::string& word) { 
            return word_analysis(word); 
        }); 
    }());

    // Nested quantification: exactly N elements satisfy complex conditions
    auto moderately_sized = [](int x) { 
        return pred::and_{
            pred::is_greater{},
            pred::is_less{}
        }(x, 3, 8);  // 3 < x < 8, so x in {4, 5, 6, 7}
    };
    
    suite.test("exactly 4 numbers are moderately sized", [&](){ 
        return pred::exactly_n_elements_satisfy{}(numbers, 4, moderately_sized); 
    }());

    // Permutation testing with complex predicates
    std::vector<int> scrambled = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    suite.test("scrambled is permutation of numbers", numbers, pred::is_permutation_of{}, scrambled);
    
    auto maintains_property = pred::and_{
        [&](const auto& vec1, const auto& vec2) { return pred::is_permutation_of{}(vec1, vec2); },
        [](const auto& vec1, const auto& vec2) { return pred::containers_equal{}(
            [&vec1]() { 
                auto copy = vec1; 
                std::sort(copy.begin(), copy.end()); 
                return copy; 
            }(),
            [&vec2]() { 
                auto copy = vec2; 
                std::sort(copy.begin(), copy.end()); 
                return copy; 
            }()
        ); }
    };
    
    suite.test("scrambled maintains sorted equality with numbers", [&](){ 
        return maintains_property(numbers, scrambled); 
    }());

    suite.suite_summary();
    return 0;
}
