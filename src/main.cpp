#include "../include/evaluator_all.h"
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

// test 1
template<typename T>
bool is_equal_func(const T lhs, const T rhs)
{
    return lhs == rhs;
}

// test 2
namespace someSpace
{
template<typename T>
bool is_greater_equal(const T& lhs, const T& rhs)
{
    return std::greater_equal<T>{}(lhs, rhs);
}

} // namespace someSpace

// test 3
struct IsEqualFunctor
{
    template<typename T>
    bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs == rhs;
    }
};

// test 4
auto is_equal_lambda = [](const auto& lhs, const auto& rhs) {
    return tunit::predicates::is_equal{}(lhs, rhs);
};

int main()
{
    tunit::Evaluator test1(1, 1, is_equal_func);
    tunit::Evaluator test2(1, 1, someSpace::is_greater_equal<int>); // ugly implicit deduction
    // to avoid implicit function predicates:
    int a{}, b{};
    tunit::Evaluator testDecla(a, b,
                               someSpace::is_greater_equal<decltype(a)>); // better but still ugly

    tunit::Evaluator test3(1, 1, IsEqualFunctor{});
    tunit::Evaluator test4(1, 1, is_equal_lambda);
    tunit::Evaluator test5(1, 1, [](const auto& lhs, const auto& rhs) { return lhs == rhs; });

    // NEW: Using the functor predicates directly - this is the cleanest approach!
    tunit::Evaluator test6(1, 1, tunit::predicates::is_equal{});
    tunit::Evaluator test7(10, 5, tunit::predicates::is_greater{});
    tunit::Evaluator test8(3, 7, tunit::predicates::is_less{});

    std::cout << "\ntest1 (template function): " << test1();
    std::cout << "\ntest2 (namespace::template_function): " << test2();
    std::cout << "\ntest3 (functor: recommended option): " << test3();
    std::cout << "\ntest4 (lambda wrapper around namespace::template_func): " << test4();
    std::cout << "\ntest5 (inline lambda): " << test5.evaluate();
    std::cout << "\ntest6 (NEW: direct functor usage): " << test6();
    std::cout << "\ntest7 (NEW: is_greater functor): " << test7();
    std::cout << "\ntest8 (NEW: is_less functor): " << test8();

    return 0;
}