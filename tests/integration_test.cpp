#include "tUnit.h"
#include <algorithm>
#include <string>
#include <vector>

namespace
{
namespace pred = tUnit::predicates;

auto &suite = tUnit::Orchestrator::instance().get_suite("Integration Tests");

void test_complex_container_logic()
{
  auto &test = suite.get_test("Complex Container Logic");
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  bool not_empty_and_min_size = pred::is_not_empty{}(numbers) && pred::has_min_size{}(numbers, 5);
  bool all_positive = pred::all_elements_satisfy{}(numbers, pred::is_positive{});
  bool contains_zero = pred::contains_element{}(numbers, 0);
  bool all_positive_or_contains_zero = all_positive || contains_zero;
  bool complex_container_result = not_empty_and_min_size && all_positive_or_contains_zero;
  test.expect("numbers satisfies complex container logic", complex_container_result, true);
}

void test_numeric_predicates_with_containers()
{
  auto &test = suite.get_test("Numeric Predicates With Containers");
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  bool has_even_positive = false;
  for (int x : numbers)
  {
    if (pred::is_positive{}(x) && pred::is_even{}(x))
    {
      has_even_positive = true;
      break;
    }
  }
  test.expect("numbers has elements satisfying even_and_positive", has_even_positive, true);
}

void test_string_and_container_integration()
{
  auto &test = suite.get_test("String and Container Integration");
  std::vector<std::string> words = {"hello", "world", "test", "integration"};
  bool has_long_word = false;
  for (const std::string &word : words)
  {
    if (pred::has_min_size{}(word, 5))
    {
      has_long_word = true;
      break;
    }
  }
  test.expect("words contains long word", has_long_word, true);
}

void test_conditional_logic_with_containers()
{
  auto &test = suite.get_test("Conditional Logic With Containers");
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::vector<int> empty_vec = {};
  bool conditional_numbers = pred::is_empty{}(numbers) ? true : pred::is_sorted{}(numbers);
  bool conditional_empty = pred::is_empty{}(empty_vec) ? true : pred::is_sorted{}(empty_vec);
  test.expect("numbers satisfies conditional (empty->valid | non-empty->sorted)", conditional_numbers, true);
  test.expect("empty_vec satisfies conditional", conditional_empty, true);
}

void test_range_predicates_with_containers()
{
  auto &test = suite.get_test("Range Predicates With Containers");
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  bool all_in_range = true;
  for (int x : numbers)
  {
    if (!pred::is_in_range{}(x, 1, 10))
    {
      all_in_range = false;
      break;
    }
  }
  test.expect("all numbers are in valid range 1-10", all_in_range, true);
}

void test_complex_logical_combinations()
{
  auto &test = suite.get_test("Complex Logical Combinations");
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  bool result = pred::is_not_empty{}(numbers) && pred::all_elements_satisfy{}(numbers, pred::is_positive{});
  test.expect("complex logical combination", result, true);
}

struct IntegrationTestRunner
{
  IntegrationTestRunner()
  {
    test_complex_container_logic();
    test_numeric_predicates_with_containers();
    test_string_and_container_integration();
    test_conditional_logic_with_containers();
    test_range_predicates_with_containers();
    test_complex_logical_combinations();
  }
};

static IntegrationTestRunner runner;

} // anonymous namespace
