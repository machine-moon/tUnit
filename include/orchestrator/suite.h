#pragma once

#include "test.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace tunit {

class Suite {
private:
  std::string name_;
  std::size_t passes_;
  std::size_t fails_;

  Suite(const std::string &name);

public:
  Test &get_test(const std::string &test_name);

  std::size_t get_passes_count() const;
  std::size_t get_fails_count() const;
  const std::string &get_name() const;

  // test results
  void record_result();

  friend class Orchestrator;
};

} // namespace tunit
