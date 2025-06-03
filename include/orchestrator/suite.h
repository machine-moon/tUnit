#pragma once
#include <string>
#include <unordered_map>
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
  std::unordered_map<std::string, Test*> tests_; 


  friend class Orchestrator;
};

} // namespace tunit