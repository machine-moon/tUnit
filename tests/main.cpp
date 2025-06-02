#include "tsuite.h"

int main() {
    // Run all test suites
    // Note: Individual test files will register themselves with the runner
    
    return tunit::Runner::all_tests_passed() ? 0 : 1;
}