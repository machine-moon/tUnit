#include <algorithm>
#include <string>
#include <vector>

#include "../include/tunit.h"

namespace pred = tunit::predicates;

int main() {
  auto& suite = tunit::Runner::get_suite("Integration Tests");

  // Test data
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::vector<std::string> words = {"hello", "world", "test", "integration"};

  // Combining logical, complex, and container predicates
  bool not_empty_and_min_size = pred::is_not_empty{}(numbers) && pred::has_min_size{}(numbers, 5);
  bool all_positive = pred::all_elements_satisfy{}(numbers, pred::is_positive{});
  bool contains_zero = pred::contains_element{}(numbers, 0);
  bool all_positive_or_contains_zero = all_positive || contains_zero;
  bool complex_container_result = not_empty_and_min_size && all_positive_or_contains_zero;

  suite.test("numbers satisfies complex container logic", complex_container_result);

  // ombining numeric predicates with container operations
  bool has_even_positive = false;
  for (int x : numbers) {
    if (pred::is_positive{}(x) && pred::is_even{}(x)) {
      has_even_positive = true;
      break;
    }
  }
  suite.test("numbers has elements satisfying even_and_positive", has_even_positive);

  // String and container integration
  bool has_long_word = false;
  for (const std::string& word : words) {
    if (pred::has_min_size{}(word, 5)) {
      has_long_word = true;
      break;
    }
  }
  suite.test("words contains long word", has_long_word);

  // Conditional logic with containers
  std::vector<int> empty_vec = {};
  bool conditional_numbers = pred::is_empty{}(numbers) ? true : pred::is_sorted{}(numbers);
  bool conditional_empty = pred::is_empty{}(empty_vec) ? true : pred::is_sorted{}(empty_vec);

  suite.test("numbers satisfies conditional (empty->valid | non-empty->sorted)", conditional_numbers);
  suite.test("empty_vec satisfies conditional", conditional_empty);

  // Range predicates with container elements
  bool all_in_range = true;
  for (int x : numbers) {
    if (!pred::is_in_range{}(x, 1, 10)) {
      all_in_range = false;
      break;
    }
  }
  suite.test("all numbers are in valid range 1-10", all_in_range);

  // Complex logical combinations
  bool found_complex_pair = false;
  for (size_t i = 0; i < numbers.size() - 1; ++i) {
    int a = numbers[i];
    int b = numbers[i + 1];

    bool even_and_greater = pred::is_even{}(a) && pred::is_greater{}(a, b);
    bool odd_and_less = pred::is_odd{}(a) && pred::is_less{}(a, b);

    if (even_and_greater || odd_and_less) {
      found_complex_pair = true;
      break;
    }
  }
  suite.test("numbers contain pairs satisfying complex logic", found_complex_pair);

  // String predicates with complex logic
  bool all_words_satisfy_analysis = true;
  for (const std::string& word : words) {
    bool min_size = pred::has_min_size{}(word, 3);
    bool starts_with_expected = pred::starts_with{}(word, std::string("h")) || pred::starts_with{}(word, std::string("w")) || pred::starts_with{}(word, std::string("t")) || pred::starts_with{}(word, std::string("i"));
    if (!(min_size && starts_with_expected)) {
      all_words_satisfy_analysis = false;
      break;
    }
  }
  suite.test("all words satisfy analysis criteria", all_words_satisfy_analysis);

  // Nested quantification: exactly N elements satisfy complex conditions
  int moderately_sized_count = 0;
  for (int x : numbers) {
    if (pred::is_greater{}(x, 3) && pred::is_less{}(x, 8)) {
      moderately_sized_count++;
    }
  }
  suite.test("exactly 4 numbers are moderately sized", moderately_sized_count == 4);

  // Permutation testing with complex predicates
  std::vector<int> scrambled = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  suite.test("scrambled is permutation of numbers", numbers, pred::is_permutation_of{}, scrambled);

  // Maintains property test
  bool is_permutation = pred::is_permutation_of{}(numbers, scrambled);
  auto sorted1 = numbers;
  auto sorted2 = scrambled;
  std::sort(sorted1.begin(), sorted1.end());
  std::sort(sorted2.begin(), sorted2.end());
  bool sorted_equal = pred::containers_equal{}(sorted1, sorted2);
  bool maintains_property_result = is_permutation && sorted_equal;

  suite.test("scrambled maintains sorted equality with numbers", maintains_property_result);

  return tunit::Runner::all_tests_passed() ? 0 : 1;
}
