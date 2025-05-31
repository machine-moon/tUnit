#include "../include/evaluator_all.h"
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <memory>
#include <functional>

namespace pred = tunit::predicates;

// Custom classes for testing
struct Person {
    std::string name;
    int age;
    bool is_admin;
    
    Person(std::string n, int a, bool admin = false) : name(std::move(n)), age(a), is_admin(admin) {}
    
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age && is_admin == other.is_admin;
    }
};

struct Point {
    double x, y;
    Point(double x_, double y_) : x(x_), y(y_) {}
    
    double distance_from_origin() const {
        return std::sqrt(x*x + y*y);
    }
    
    bool operator<(const Point& other) const {
        return distance_from_origin() < other.distance_from_origin();
    }
};

int main()
{
    tunit::TestRunner runner("Integration & Real-World Usage Tests");

    std::cout << "=== Testing Integration & Real-World Scenarios ===" << std::endl;

    // ==================== API Design Showcase ====================
    std::cout << "\n--- API Design Showcase ---" << std::endl;

    {
        // Showcase different ways to use the evaluator API
        
        // 1. Function pointer approach
        auto func_ptr = [](int a, int b) { return a > b; };
        tunit::Evaluator eval1(10, 5, func_ptr);
        runner.test("Function pointer approach", eval1());
        
        // 2. Functor approach
        struct CustomComparator {
            bool operator()(int a, int b) const { return a != b; }
        };
        tunit::Evaluator eval2(10, 5, CustomComparator{});
        runner.test("Functor approach", eval2());
        
        // 3. Lambda approach
        auto lambda = [](int a, int b) { return a % b == 0; };
        tunit::Evaluator eval3(10, 5, lambda);
        runner.test("Lambda approach", eval3());
        
        // 4. Standard library predicates
        tunit::Evaluator eval4(10, 5, std::greater<int>{});
        runner.test("Standard library predicate", eval4());
        
        // 5. Our custom predicates (cleanest)
        tunit::Evaluator eval5(10, 5, pred::is_greater{});
        runner.test("Custom predicate (cleanest)", eval5());
    }

    // ==================== Complex Business Logic ====================
    std::cout << "\n--- Complex Business Logic ---" << std::endl;

    {
        // User validation system
        auto valid_adult_user = pred::all_of{
            [](const Person& p) { return p.age >= 18; },              // Must be adult
            [](const Person& p) { return !p.name.empty(); },          // Must have name
            pred::or_{                                                  // Either:
                [](const Person& p) { return p.is_admin; },           //   Is admin, OR
                pred::all_of{                                          //   All of:
                    [](const Person& p) { return p.age >= 21; },      //     21+ years old
                    [](const Person& p) { return p.name.length() >= 2; } //     Valid name length
                }
            }
        };
        
        Person admin_teen("Alice", 17, true);
        Person valid_adult("Bob", 25, false);
        Person young_adult("Charlie", 19, false);
        Person child("Dave", 12, false);
        
        runner.test("Business logic: admin teen", valid_adult_user(admin_teen));
        runner.test("Business logic: valid adult", valid_adult_user(valid_adult));
        runner.test("Business logic: young adult", !valid_adult_user(young_adult));
        runner.test("Business logic: child", !valid_adult_user(child));
    }

    {
        // Data validation pipeline
        std::vector<int> data = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
        
        auto data_quality_check = pred::all_of{
            pred::is_not_empty{},                    // Has data
            pred::has_min_size{},                    // Minimum size
            pred::all_elements_satisfy{},            // All elements valid
            pred::is_sorted{},                       // Properly ordered
            pred::is_unique{}                        // No duplicates
        };
        
        // Note: This is conceptual - actual implementation would need adaptation
        runner.test("Data validation: non-empty", pred::is_not_empty{}(data));
        runner.test("Data validation: min size", pred::has_min_size{}(data, 5));
        runner.test("Data validation: sorted", pred::is_sorted{}(data));
        runner.test("Data validation: unique", pred::is_unique{}(data));
    }

    // ==================== Mathematical Operations ====================
    std::cout << "\n--- Mathematical Operations ---" << std::endl;

    {
        // Vector operations
        std::vector<Point> points = {
            Point(0, 0), Point(1, 1), Point(2, 2), Point(3, 3)
        };
        
        auto all_in_first_quadrant = [](const std::vector<Point>& pts) {
            return pred::all_elements_satisfy{}(pts, [](const Point& p) {
                return p.x >= 0 && p.y >= 0;
            });
        };
        
        auto sorted_by_distance = [](const std::vector<Point>& pts) {
            return pred::is_sorted{}(pts);
        };
        
        runner.test("Math: all in first quadrant", all_in_first_quadrant(points));
        runner.test("Math: sorted by distance", sorted_by_distance(points));
        
        // Range validation
        auto valid_range = pred::all_of{
            pred::is_positive{},
            [](double x) { return x <= 100.0; }
        };
        
        runner.test("Math: valid range positive", valid_range(50.0));
        runner.test("Math: valid range negative", !valid_range(-10.0));
        runner.test("Math: valid range too large", !valid_range(150.0));
    }

    // ==================== String Processing ====================
    std::cout << "\n--- String Processing ---" << std::endl;

    {
        // Email validation
        auto valid_email_format = pred::all_of{
            pred::contains_substring{},              // Contains @
            pred::not_{pred::starts_with{}},         // Doesn't start with @
            pred::not_{pred::ends_with{}},           // Doesn't end with @
            [](const std::string& email, const std::string&) {
                return email.find('@') != email.rfind('@'); // Only one @
            }
        };
        
        // Note: This is simplified - real email validation is much more complex
        std::string valid_email = "user@domain.com";
        std::string invalid_email1 = "@domain.com";
        std::string invalid_email2 = "user@";
        
        runner.test("Email: contains @", pred::contains_substring{}(valid_email, "@"));
        runner.test("Email: doesn't start with @", !pred::starts_with{}(valid_email, "@"));
        runner.test("Email: doesn't end with @", !pred::ends_with{}(valid_email, "@"));
        
        runner.test("Email: invalid start", pred::starts_with{}(invalid_email1, "@"));
        runner.test("Email: invalid end", pred::ends_with{}(invalid_email2, "@"));
    }

    {
        // Text analysis
        std::vector<std::string> words = {"hello", "world", "test", "string", "processing"};
        
        auto has_long_words = pred::any_element_satisfies{};
        auto all_lowercase = pred::all_elements_satisfy{};
        auto contains_test = pred::contains_element{};
        
        runner.test("Text: has long words", 
                   has_long_words(words, [](const std::string& word) { return word.length() > 5; }));
        runner.test("Text: all lowercase", 
                   all_lowercase(words, [](const std::string& word) {
                       return std::all_of(word.begin(), word.end(), [](char c) { return std::islower(c); });
                   }));
        runner.test("Text: contains test", contains_test(words, std::string("test")));
    }

    // ==================== Performance and Efficiency ====================
    std::cout << "\n--- Performance and Efficiency ---" << std::endl;

    {
        // Large dataset processing
        std::vector<int> large_dataset(100000);
        std::iota(large_dataset.begin(), large_dataset.end(), 1);
        
        // Benchmark different approaches
        runner.benchmark("Large dataset: contains element", [&]() {
            return pred::contains_element{}(large_dataset, 50000);
        }, 100);
        
        runner.benchmark("Large dataset: is sorted", [&]() {
            return pred::is_sorted{}(large_dataset);
        }, 100);
        
        runner.benchmark("Large dataset: all positive", [&]() {
            return pred::all_elements_satisfy{}(large_dataset, pred::is_positive{});
        }, 10);
        
        // Compare with evaluator wrapper
        tunit::Evaluator eval(large_dataset, 50000, pred::contains_element{});
        runner.benchmark("Large dataset: evaluator wrapper", [&]() {
            return eval();
        }, 100);
    }

    // ==================== Error Handling and Edge Cases ====================
    std::cout << "\n--- Error Handling and Edge Cases ---" << std::endl;

    {
        // Empty containers
        std::vector<int> empty_vec;
        std::string empty_str;
        
        runner.test("Edge: empty vector not empty", !pred::is_not_empty{}(empty_vec));
        runner.test("Edge: empty string not empty", !pred::is_not_empty{}(empty_str));
        runner.test("Edge: empty vector sorted", pred::is_sorted{}(empty_vec));
        runner.test("Edge: empty vector unique", pred::is_unique{}(empty_vec));
        
        // Single element containers
        std::vector<int> single = {42};
        runner.test("Edge: single element sorted", pred::is_sorted{}(single));
        runner.test("Edge: single element unique", pred::is_unique{}(single));
        runner.test("Edge: single element contains itself", pred::contains_element{}(single, 42));
    }

    {
        // Extreme values
        int max_int = std::numeric_limits<int>::max();
        int min_int = std::numeric_limits<int>::min();
        
        runner.test_evaluator("Edge: max int comparison", max_int, max_int-1, pred::is_greater{}, true);
        runner.test_evaluator("Edge: min int comparison", min_int, min_int+1, pred::is_less{}, true);
        
        // Floating point edge cases
        double zero = 0.0;
        double neg_zero = -0.0;
        runner.test_evaluator("Edge: positive/negative zero", zero, neg_zero, pred::is_equal{}, true);
    }

    // ==================== Type Safety and Template Usage ====================
    std::cout << "\n--- Type Safety and Template Usage ---" << std::endl;

    {
        // Mixed type comparisons
        runner.test_evaluator("Types: int/double", 5, 5.0, pred::is_equal{}, true);
        runner.test_evaluator("Types: char/int", 'A', 65, pred::is_equal{}, true);
        
        // String types
        std::string std_str = "hello";
        const char* c_str = "hello";
        std::string_view str_view = "hello";
        
        runner.test_evaluator("Types: string types equal", std_str, c_str, pred::is_equal{}, true);
        runner.test("Types: contains with string_view", pred::contains_substring{}(str_view, "ell"));
    }

    // ==================== Practical Usage Patterns ====================
    std::cout << "\n--- Practical Usage Patterns ---" << std::endl;

    {
        // Configuration validation
        struct Config {
            int port;
            std::string host;
            bool ssl_enabled;
            std::vector<std::string> allowed_origins;
        };
        
        Config config{8080, "localhost", true, {"http://localhost:3000", "https://example.com"}};
        
        auto valid_port = pred::and_{
            pred::is_positive{},
            [](int port) { return port <= 65535; }
        };
        
        auto valid_host = pred::and_{
            pred::is_not_empty{},
            pred::not_{pred::contains_substring{}}  // No spaces
        };
        
        runner.test("Config: valid port", valid_port(config.port));
        runner.test("Config: valid host", pred::is_not_empty{}(config.host));
        runner.test("Config: has origins", pred::is_not_empty{}(config.allowed_origins));
    }

    {
        // State machine validation
        enum class State { Initial, Loading, Ready, Error };
        
        auto valid_transition = [](State from, State to) {
            using pred::or_, pred::and_, pred::is_equal;
            
            return or_{
                and_{is_equal{}(from, State::Initial), is_equal{}(to, State::Loading)},
                and_{is_equal{}(from, State::Loading), or_{is_equal{}(to, State::Ready), is_equal{}(to, State::Error)}},
                and_{is_equal{}(from, State::Ready), is_equal{}(to, State::Error)},
                is_equal{}(from, to)  // Self-transition always valid
            }(from, to);
        };
        
        runner.test("State: valid transition Initial->Loading", 
                   valid_transition(State::Initial, State::Loading));
        runner.test("State: invalid transition Initial->Ready", 
                   !valid_transition(State::Initial, State::Ready));
        runner.test("State: valid transition Loading->Error", 
                   valid_transition(State::Loading, State::Error));
    }

    // ==================== Composition and Reusability ====================
    std::cout << "\n--- Composition and Reusability ---" << std::endl;

    {
        // Reusable validation components
        auto is_valid_identifier = pred::and_{
            pred::is_not_empty{},
            [](const std::string& s) { return std::isalpha(s[0]); },  // Starts with letter
            [](const std::string& s) {  // Only alphanumeric and underscore
                return std::all_of(s.begin(), s.end(), [](char c) {
                    return std::isalnum(c) || c == '_';
                });
            }
        };
        
        auto is_reasonable_length = pred::and_{
            [](const std::string& s) { return s.length() >= 1; },
            [](const std::string& s) { return s.length() <= 50; }
        };
        
        auto complete_validation = pred::and_{is_valid_identifier, is_reasonable_length};
        
        runner.test("Composition: valid identifier", is_valid_identifier(std::string("valid_name")));
        runner.test("Composition: invalid start", !is_valid_identifier(std::string("123invalid")));
        runner.test("Composition: complete validation", complete_validation(std::string("good_name")));
    }

    // ==================== Advanced Use Cases ====================
    std::cout << "\n--- Advanced Use Cases ---" << std::endl;

    {
        // Generic algorithm with custom predicates
        auto process_data = [](const auto& container, auto predicate) {
            return pred::all_of{
                pred::is_not_empty{},
                [predicate](const auto& c) { return pred::all_elements_satisfy{}(c, predicate); }
            }(container);
        };
        
        std::vector<int> numbers = {2, 4, 6, 8, 10};
        std::vector<std::string> words = {"hello", "world", "test"};
        
        runner.test("Generic: all even numbers", 
                   process_data(numbers, [](int x) { return x % 2 == 0; }));
        runner.test("Generic: all short words", 
                   process_data(words, [](const std::string& s) { return s.length() <= 5; }));
    }

    runner.summary();
    return runner.all_passed() ? 0 : 1;
}
