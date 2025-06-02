#pragma once

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace tunit {
namespace trace {

#define TUNIT_TRACE_SCOPE(name) tunit::trace::TraceScope _trace_scope_##__LINE__(name)  // custom message for each scope
#define TUNIT_TRACE_FUNCTION() TUNIT_TRACE_SCOPE(__func__)

class TraceScope;

class TracedException : public std::runtime_error {
 private:
  std::vector<std::string> trace_stack_;
  std::string original_message_;
  mutable std::string full_message_;

 public:
  TracedException(const std::string& message) : std::runtime_error(message), original_message_(message) {}

  void add_trace(const std::string& context) {
    trace_stack_.push_back(context);
    full_message_.clear();
  }

  const std::vector<std::string>& get_trace_stack() const { return trace_stack_; }

  const std::string& get_original_message() const { return original_message_; }

  const char* what() const noexcept override {
    if (full_message_.empty()) {
      update_message();
    }
    return full_message_.c_str();
  }

 private:
  void update_message() const {
    std::ostringstream oss;
    oss << "Error: " << original_message_ << "\n";
    if (!trace_stack_.empty()) {
      oss << "Trace (most recent first):\n";
      for (auto it = trace_stack_.rbegin(); it != trace_stack_.rend(); ++it) {
        oss << "  -> " << *it << "\n";
      }
    }
    full_message_ = oss.str();
  }
};

class TraceContext {
 private:
  static thread_local std::vector<std::string> current_trace_;

 public:
  static void push_scope(const std::string& scope_name) { current_trace_.push_back(scope_name); }

  static void pop_scope() {
    if (!current_trace_.empty()) {
      current_trace_.pop_back();
    }
  }

  static std::vector<std::string> get_current_trace() { return current_trace_; }

  static void enrich_exception(TracedException& ex) {
    for (const auto& scope : current_trace_) {
      ex.add_trace(scope);
    }
  }
};

class TraceScope {
 private:
  std::string scope_name_;

 public:
  TraceScope(const std::string& scope_name) : scope_name_(scope_name) { TraceContext::push_scope(scope_name_); }

  ~TraceScope() { TraceContext::pop_scope(); }

  // Non-copyable/movable for safety
  TraceScope(const TraceScope&) = delete;
  TraceScope& operator=(const TraceScope&) = delete;
  TraceScope(TraceScope&&) = delete;
  TraceScope& operator=(TraceScope&&) = delete;
};

// Automatic trace enrichment
template <typename... Args>
[[noreturn]] void throw_traced(Args&&... args) {
  try {
    TracedException ex(std::forward<Args>(args)...);
    TraceContext::enrich_exception(ex);
    throw ex;
  } catch (TracedException& ex) {
    // Only enrich if there is a parent scope above (i.e., current_trace_ is not empty)
    if (!TraceContext::get_current_trace().empty()) {
      TraceContext::enrich_exception(ex);
    }
    printf("TracedException: %s\n", ex.what());
    throw;  // program will terminate here
  }
}

}  // namespace trace
}  // namespace tunit
