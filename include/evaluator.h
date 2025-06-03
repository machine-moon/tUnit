#pragma once

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace tunit
{

template <typename T, typename U = T, typename P = bool (*)(const T &, const U &)>
class Evaluator
{
private:
  T lhs_;
  U rhs_;
  P pred_;

public:
  constexpr Evaluator(const T &lhs, const U &rhs, P pred) noexcept : lhs_(lhs), rhs_(rhs), pred_(pred) {}

  bool use(const T &lhs, const U &rhs) const { return pred_(lhs, rhs); }
  bool compare(const U &rhs) const { return pred_(lhs_, rhs); }
  bool evaluate() const { return pred_(lhs_, rhs_); }
  bool operator()() const { return this->evaluate(); }
};

} // namespace tunit
