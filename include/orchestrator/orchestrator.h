#pragma once

#include "suite.h"
#include "test.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace tunit {

class Orchestrator {
private:
  std::unordered_map<std::string, Suite *> suites_;
  std::unordered_map<Suite *, std::unordered_map<std::string, Test *>> tests_;
  std::map<Test *, bool> assertion_results_;

  std::size_t total_passes_;
  std::size_t total_fails_;
  static std::unique_ptr<Orchestrator> instance_;

  Orchestrator();

public:
  static Orchestrator &get_instance();
  Suite &register_suite(const std::string &suite_name);
  Test &register_test(Suite *suite, const std::string &test_name);
  void register_result(Test *test, bool result);

  Test &get_test(const std::string &test_name);
  Suite &get_suite(const std::string &suite_name);

  bool all_suites_passed() const;
  std::size_t get_total_passes_count() const;
  std::size_t get_total_fails_count() const;

  void record_test_result(const std::string &file_name);
};

} // namespace tunit
