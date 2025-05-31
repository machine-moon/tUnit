#include "../include/evaluator_all.h"
#include <iostream>
#include <vector>

namespace pred = tunit::predicates;

int main() {
    std::cout << "=== Basic TUnit Test ===" << std::endl;
    
    // Test 1: Basic numeric comparisons using functor objects
    tunit::Evaluator eq_eval(5, 5, pred::is_equal{});
    std::cout << "5 == 5: " << (eq_eval() ? "PASS" : "FAIL") << std::endl;
    
    tunit::Evaluator gt_eval(10, 5, pred::is_greater{});
    std::cout << "10 > 5: " << (gt_eval() ? "PASS" : "FAIL") << std::endl;
    
    tunit::Evaluator lt_eval(3, 7, pred::is_less{});
    std::cout << "3 < 7: " << (lt_eval() ? "PASS" : "FAIL") << std::endl;
    
    // Test 2: Single argument predicates using operator()
    pred::is_even even_pred{};
    std::cout << "4 is even: " << (even_pred(4) ? "PASS" : "FAIL") << std::endl;
    std::cout << "5 is even: " << (!even_pred(5) ? "PASS" : "FAIL") << std::endl;
    
    pred::is_positive pos_pred{};
    std::cout << "5 is positive: " << (pos_pred(5) ? "PASS" : "FAIL") << std::endl;
    std::cout << "-3 is positive: " << (!pos_pred(-3) ? "PASS" : "FAIL") << std::endl;
    
    // Test 3: Container predicates
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    pred::contains_element cont_pred{};
    std::cout << "vector contains 3: " << (cont_pred(numbers, 3) ? "PASS" : "FAIL") << std::endl;
    std::cout << "vector contains 10: " << (!cont_pred(numbers, 10) ? "PASS" : "FAIL") << std::endl;
    
    pred::has_size size_pred{};
    std::cout << "vector has size 5: " << (size_pred(numbers, 5) ? "PASS" : "FAIL") << std::endl;
    
    return 0;
}
