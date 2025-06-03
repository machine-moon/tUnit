#pragma once
#include <string>
#include <vector>

namespace tunit {

class Test;

class Suite {
public:
  Suite(const std::string &name);

  Test &get_test(const std::string &test_name);

  const std::string &name() const;

private:
  std::string name_;
  std::vector<std::string>
      test_names_; // Store test names or IDs for mapping in Orchestrator

  friend class Orchestrator;
};

} // namespace tunit