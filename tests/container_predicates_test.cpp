#include <array>
#include <list>
#include <string>
#include <vector>

#include "../include/tunit.h"

namespace pred = tunit::predicates;

int main() {
  auto& suite = tunit::Runner::get_suite("Container Predicates");

  // Test data
  std::vector<int> numbers = {1, 2, 3, 4, 5};
  std::vector<int> empty_vec = {};
  std::vector<int> even_numbers = {2, 4, 6, 8};
  std::vector<int> mixed_numbers = {1, 2, 3, 4};
  std::vector<int> sorted_vec = {1, 2, 3, 4, 5};
  std::vector<int> reverse_sorted = {5, 4, 3, 2, 1};
  std::vector<int> unsorted = {3, 1, 4, 2, 5};
  std::vector<int> duplicates = {1, 2, 2, 3, 3};
  std::vector<int> unique_vec = {1, 2, 3, 4, 5};

  // Container element predicates
  suite.test("numbers contains 3", numbers, pred::contains_element{}, 3);
  suite.test("numbers does not contain 10", numbers, pred::contains_element{}, 10, false);

  // contains_if tests
  auto is_even = [](int x) { return x % 2 == 0; };
  suite.test("numbers contains even element", [&]() { return pred::contains_if{}(numbers, is_even); }());
  suite.test("empty_vec contains no even", [&]() { return pred::contains_if{}(empty_vec, is_even); }(), false);

  // contains_all_elements tests
  std::vector<int> subset = {1, 3, 5};
  std::vector<int> not_subset = {1, 3, 10};
  suite.test("numbers contains all of subset", numbers, pred::contains_all_elements{}, subset);
  suite.test("numbers does not contain all of not_subset", numbers, pred::contains_all_elements{}, not_subset, false);

  // contains_any_elements tests
  std::vector<int> some_match = {10, 3, 20};
  std::vector<int> no_match = {10, 20, 30};
  suite.test("numbers contains any of some_match", numbers, pred::contains_any_elements{}, some_match);
  suite.test("numbers contains none of no_match", numbers, pred::contains_any_elements{}, no_match, false);

  // Container size predicates
  suite.test("empty_vec is_empty", [&]() { return pred::is_empty{}(empty_vec); }());
  suite.test("numbers is_not_empty", [&]() { return pred::is_not_empty{}(numbers); }());
  suite.test("numbers has_size 5", [&]() { return pred::has_size{}(numbers, 5); }());
  suite.test("numbers has_min_size 3", [&]() { return pred::has_min_size{}(numbers, 3); }());
  suite.test("numbers has_max_size 10", [&]() { return pred::has_max_size{}(numbers, 10); }());
  suite.test("numbers has_size_in_range 3-7", [&]() { return pred::has_size_in_range{}(numbers, 3, 7); }());

  // Failed size tests
  suite.test("numbers does not have_size 3", [&]() { return pred::has_size{}(numbers, 3); }(), false);
  suite.test("numbers fails has_min_size 10", [&]() { return pred::has_min_size{}(numbers, 10); }(), false);
  suite.test("numbers fails has_max_size 3", [&]() { return pred::has_max_size{}(numbers, 3); }(), false);

  // Container algorithm predicates
  auto is_positive = [](int x) { return x > 0; };
  auto is_greater_than_ten = [](int x) { return x > 10; };

  suite.test("numbers all_elements_satisfy positive", [&]() { return pred::all_elements_satisfy{}(numbers, is_positive); }());
  suite.test("numbers any_element_satisfies even", [&]() { return pred::any_element_satisfies{}(numbers, is_even); }());
  suite.test("numbers no_elements_satisfy > 10", [&]() { return pred::no_elements_satisfy{}(numbers, is_greater_than_ten); }());

  // count_elements_satisfying tests
  suite.test("mixed_numbers has 2 even elements", [&]() { return pred::count_elements_satisfying{}(mixed_numbers, is_even) == 2; }());

  // exactly_n_elements_satisfy tests
  suite.test("mixed_numbers exactly 2 satisfy even", [&]() { return pred::exactly_n_elements_satisfy{}(mixed_numbers, 2, is_even); }());
  suite.test("mixed_numbers not exactly 3 satisfy even", [&]() { return pred::exactly_n_elements_satisfy{}(mixed_numbers, 3, is_even); }(), false);

  // at_least_n_elements_satisfy tests
  suite.test("mixed_numbers at_least 1 satisfies even", [&]() { return pred::at_least_n_elements_satisfy{}(mixed_numbers, 1, is_even); }());
  suite.test("mixed_numbers fails at_least 5 satisfy even", [&]() { return pred::at_least_n_elements_satisfy{}(mixed_numbers, 5, is_even); }(), false);

  // at_most_n_elements_satisfy tests
  suite.test("mixed_numbers at_most 3 satisfy even", [&]() { return pred::at_most_n_elements_satisfy{}(mixed_numbers, 3, is_even); }());
  suite.test("mixed_numbers fails at_most 1 satisfy even", [&]() { return pred::at_most_n_elements_satisfy{}(mixed_numbers, 1, is_even); }(), false);

  // Container ordering predicates
  suite.test("sorted_vec is_sorted", [&]() { return pred::is_sorted{}(sorted_vec); }());
  suite.test("reverse_sorted is_reverse_sorted", [&]() { return pred::is_reverse_sorted{}(reverse_sorted); }());
  suite.test("unsorted is not sorted", [&]() { return pred::is_sorted{}(unsorted); }(), false);
  suite.test("unique_vec is_unique", [&]() { return pred::is_unique{}(unique_vec); }());
  suite.test("duplicates is not unique", [&]() { return pred::is_unique{}(duplicates); }(), false);

  // Container equality predicates
  std::vector<int> numbers_copy = {1, 2, 3, 4, 5};
  std::vector<int> permuted = {5, 4, 3, 2, 1};
  std::vector<int> different = {1, 2, 3, 4, 6};

  suite.test("numbers equals numbers_copy", numbers, pred::containers_equal{}, numbers_copy);
  suite.test("numbers not equal to different", numbers, pred::containers_equal{}, different, false);
  suite.test("numbers is_permutation_of permuted", numbers, pred::is_permutation_of{}, permuted);
  suite.test("numbers not permutation of different", numbers, pred::is_permutation_of{}, different, false);

  // Edge cases
  std::vector<int> single_element = {42};
  suite.test("single_element is_sorted", [&]() { return pred::is_sorted{}(single_element); }());
  suite.test("single_element is_unique", [&]() { return pred::is_unique{}(single_element); }());
  suite.test("single_element has_size 1", [&]() { return pred::has_size{}(single_element, 1); }());

  return tunit::Runner::all_tests_passed() ? 0 : 1;
}
