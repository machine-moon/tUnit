#include "../include/evaluator_all.h"
#include <iostream>

using namespace tunit;
namespace pred = tunit::predicates;

int main()
{
    std::cout << "Starting debug test..." << std::endl;
    
    auto& suite = TestRunner::get_suite("Debug Test");
    std::cout << "Got test suite" << std::endl;
    
    // Simple test
    suite.test("Basic test", 5, pred::is_equal{}, 5);
    std::cout << "Completed basic test" << std::endl;
    
    suite.suite_summary();
    std::cout << "Test complete" << std::endl;
    
    return 0;
}
