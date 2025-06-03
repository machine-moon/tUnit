#include "tUnit.h"
#include <string>
#include <vector>

namespace
{
namespace pred = tUnit::predicates;

auto &suite = tUnit::Orchestrator::instance().get_suite("Container Predicates");

void test_container_element_predicates()
{
  auto &test = suite.get_test("Element Predicates");
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  std::vector<int> empty_vec = {};
  test.assert("numbers contains 3", numbers, pred::contains_element{}, 3);
  test.assert("numbers does not contain 10", numbers, pred::contains_element{}, 10);
  auto is_even = [](int x)
  { return x % 2 == 0; };
  test.expect("numbers contains even element", pred::contains_if{}(numbers, is_even), true);
  test.expect("empty_vec contains no even", pred::contains_if{}(empty_vec, is_even), false);
}

void test_contains_all_any_elements()
{
  auto &test = suite.get_test("All/Any Elements");
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  std::vector<int> subset = {1, 3, 5};
  std::vector<int> not_subset = {1, 3, 10};
  test.assert("numbers contains all of subset", numbers, pred::contains_all_elements{}, subset);
  test.assert("numbers does not contain all of not_subset", numbers, pred::contains_all_elements{}, not_subset);
  std::vector<int> some_match = {10, 3, 20};
  std::vector<int> no_match = {10, 20, 30};
  test.assert("numbers contains any of some_match", numbers, pred::contains_any_elements{}, some_match);
  test.assert("numbers contains none of no_match", numbers, pred::contains_any_elements{}, no_match);
}

void test_container_size_predicates()
{
  auto &test = suite.get_test("Size Predicates");
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  std::vector<int> empty_vec = {};
  test.expect("empty_vec is_empty", pred::is_empty{}(empty_vec), true);
  test.expect("numbers is_not_empty", pred::is_not_empty{}(numbers), true);
  test.expect("numbers has_size 5", pred::has_size{}(numbers, 5), true);
  test.expect("numbers has_min_size 3", pred::has_min_size{}(numbers, 3), true);
  test.expect("numbers has_max_size 10", pred::has_max_size{}(numbers, 10), true);
  test.expect("numbers has_size_in_range 3-7", pred::has_size_in_range{}(numbers, 3, 7), true);
  test.expect("numbers does not have_size 3", pred::has_size{}(numbers, 3), false);
  test.expect("numbers fails has_min_size 10", pred::has_min_size{}(numbers, 10), false);
  test.expect("numbers fails has_max_size 3", pred::has_max_size{}(numbers, 3), false);
}

void test_container_algorithm_predicates()
{
  auto &test = suite.get_test("Algorithm Predicates");
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  auto is_positive = [](int x)
  { return x > 0; };
  auto is_greater_than_ten = [](int x)
  { return x > 10; };
  auto is_even = [](int x)
  { return x % 2 == 0; };
  test.expect("numbers all_elements_satisfy positive", pred::all_elements_satisfy{}(numbers, is_positive), true);
  test.expect("numbers any_element_satisfies even", pred::any_element_satisfies{}(numbers, is_even), true);
  test.expect("numbers no_element_satisfies >10", pred::none_of{}(numbers, is_greater_than_ten), true);
}

struct ContainerPredicatesTestRunner
{
  ContainerPredicatesTestRunner()
  {
    test_container_element_predicates();
    test_contains_all_any_elements();
    test_container_size_predicates();
    test_container_algorithm_predicates();
  }
};

//static ContainerPredicatesTestRunner runner;

} // anonymous namespace
