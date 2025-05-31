#include "../include/evaluator_all.h"
#include <string>

using namespace tunit;
namespace pred = tunit::predicates;

int main()
{
    auto& suite = TestRunner::get_suite("Evaluator Core");

    // Basic comparison tests with v1, pred, v2 syntax
    suite.test("5 is_equal 5", 5, pred::is_equal{}, 5);
    suite.test("10 is_greater 5", 10, pred::is_greater{}, 5);
    suite.test("3 is_less 7", 3, pred::is_less{}, 7);
    suite.test("5 is_greater_equal 5", 5, pred::is_greater_equal{}, 5);
    suite.test("3 is_less_equal 7", 3, pred::is_less_equal{}, 7);
    
    // Test with different types
    suite.test("3.14 is_greater 3.0", 3.14, pred::is_greater{}, 3.0);
    suite.test("hello is_equal hello", std::string("hello"), pred::is_equal{}, std::string("hello"));
    
    // Test evaluator methods
    Evaluator eval(10, 20, pred::is_less{});
    suite.test("Evaluator operator()", eval());
    suite.test("Evaluator evaluate()", eval.evaluate());
    suite.test("Evaluator use(15, 25)", eval.use(15, 25));
    suite.test("Evaluator compare(30)", eval.compare(30));

    // Test with false cases 
    suite.test("5 is_not_equal 3", 5, pred::is_not_equal{}, 3);
    suite.test("2 is_less 10", 2, pred::is_less{}, 10);
    
    // Test negative expectations
    suite.test("5 is_not 10", 5, pred::is_equal{}, 10, false);
    suite.test("10 is_not_less 5", 10, pred::is_less{}, 5, false);

    suite.suite_summary();
    return 0;
}
