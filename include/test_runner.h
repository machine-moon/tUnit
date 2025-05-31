#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "evaluator.h"

namespace tunit
{

class TestRunner
{
private:
    std::string suite_name_;
    int suite_passes_;
    int suite_fails_;

    // Static methods to access global counters (avoiding ODR issues)
    static int& get_total_passes() { static int total_passes = 0; return total_passes; }
    static int& get_total_fails() { static int total_fails = 0; return total_fails; }
    static std::vector<std::string>& get_failed_tests() { static std::vector<std::string> failed_tests; return failed_tests; }

    // Private constructor for singleton pattern
    TestRunner(const std::string& suite_name) 
        : suite_name_(suite_name), suite_passes_(0), suite_fails_(0) {}

    void record_result(const std::string& test_name, bool passed)
    {
        if (passed)
        {
            get_total_passes()++;
            suite_passes_++;
            std::cout << "[PASS] " << suite_name_ << ": " << test_name << std::endl;
        }
        else
        {
            get_total_fails()++;
            suite_fails_++;
            get_failed_tests().push_back(suite_name_ + ": " + test_name);
            std::cout << "[FAIL] " << suite_name_ << ": " << test_name << std::endl;
        }
    }

public:
    // Singleton pattern - get test suite instance
    static TestRunner& get_suite(const std::string& suite_name)
    {
        static std::vector<std::unique_ptr<TestRunner>> suites;
        
        // Check if suite already exists
        for (auto& suite : suites)
        {
            if (suite->suite_name_ == suite_name)
            {
                return *suite;
            }
        }
        
        // Create new suite
        suites.push_back(std::unique_ptr<TestRunner>(new TestRunner(suite_name)));
        return *suites.back();
    }

    // Main test method with v1, pred, v2 syntax
    template<typename T, typename U, typename P>
    void test(const std::string& test_name, const T& v1, P pred, const U& v2, bool expected = true)
    {
        Evaluator evaluator(v1, v2, pred);
        bool result = evaluator();
        assert(result == expected);
        record_result(test_name, result == expected);
    }

    // Simple boolean test
    void test(const std::string& test_name, bool condition)
    {
        assert(condition == true);
        record_result(test_name, condition);
    }

    // Boolean test with expected value
    void test(const std::string& test_name, bool condition, bool expected)
    {
        assert(condition == expected);
        record_result(test_name, condition == expected);
    }

    // Suite summary
    void suite_summary() const
    {
        std::cout << "\n--- " << suite_name_ << " Summary ---" << std::endl;
        std::cout << "Passed: " << suite_passes_ << ", Failed: " << suite_fails_ << std::endl;
    }

    // Global summary
    static void global_summary()
    {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "GLOBAL TEST SUMMARY" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "Total Passes: " << get_total_passes() << std::endl;
        std::cout << "Total Fails:  " << get_total_fails() << std::endl;
        std::cout << "Total Tests:  " << (get_total_passes() + get_total_fails()) << std::endl;
        
        if (get_total_fails() > 0)
        {
            std::cout << "\nFailed Tests:" << std::endl;
            for (const auto& test : get_failed_tests())
            {
                std::cout << "  - " << test << std::endl;
            }
        }
        
        double pass_rate = get_total_passes() + get_total_fails() > 0 ? 
            (static_cast<double>(get_total_passes()) / (get_total_passes() + get_total_fails())) * 100.0 : 0.0;
        std::cout << "Pass Rate: " << pass_rate << "%" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }

    static bool all_tests_passed()
    {
        return get_total_fails() == 0 && get_total_passes() > 0;
    }

    static void reset_global_stats()
    {
        get_total_passes() = 0;
        get_total_fails() = 0;
        get_failed_tests().clear();
    }

    static void reset_global_counters()
    {
        reset_global_stats();
    }

    // Public accessors for global counters
    static int get_total_passes_count() { return get_total_passes(); }
    static int get_total_fails_count() { return get_total_fails(); }
};

} // namespace tunit
