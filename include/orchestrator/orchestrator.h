#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace tunit
{

class Suite;
class Test;
class Assertion;

class Orchestrator
{
public:
  static Orchestrator &instance();

  Suite &get_suite(const std::string &name);
  Test &get_test(const std::string &suite_name, const std::string &test_name);
  void log_assertion(const std::string &suite_name, const std::string &test_name, Assertion &&assertion);

  bool all_tests_passed() const;
  size_t total_assertions() const;
  size_t failed_assertions() const;
  void print_summary() const;

  const std::unordered_map<std::string, std::unique_ptr<Suite>> &suites() const;
  const std::unordered_map<std::string, std::unique_ptr<Test>> &tests() const;
  const std::vector<Assertion> &assertions_for(const std::string &suite_name, const std::string &test_name) const;

private:
  Orchestrator() = default;
  ~Orchestrator();
  
  // non-copyable/movable
  Orchestrator(const Orchestrator &) = delete;
  Orchestrator(Orchestrator &&) = delete;
  Orchestrator &operator=(const Orchestrator &) = delete;
  Orchestrator &operator=(Orchestrator &&) = delete;

  std::unordered_map<std::string, std::unique_ptr<Suite>> suites_;
  std::unordered_map<std::string, std::unique_ptr<Test>> tests_;
  std::unordered_map<std::string, std::vector<Assertion>> assertions_;

  static Orchestrator *instance_;
};

} // namespace tunit