#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace tunit {
namespace trace {

#define TUNIT_SCOPED_TRACE(message)                                            \
  do {                                                                         \
    tunit::trace::ScopedTrace _scoped_trace_##__LINE__(__FILE__, __LINE__,     \
                                                       (message));             \
  } while (0)

#define TUNIT_TRACE_SCOPE(name)                                                \
  tunit::trace::ScopedTrace _trace_scope_##__LINE__(__FILE__, __LINE__, (name))

#define TUNIT_TRACE_FUNCTION() TUNIT_TRACE_SCOPE(__func__)

struct TraceInfo {
  std::string file;
  std::int32_t line;
  std::string message;

  TraceInfo(const std::string &file_path, std::int32_t line_num,
            const std::string &msg)
      : file(file_path), line(line_num), message(msg) {}

  std::string to_string() const {
    std::ostringstream oss;
    if (!file.empty()) {
      oss << file << ":" << line << ": " << message;
    } else {
      oss << message;
    }
    return oss.str();
  }
};

class TracedException : public std::runtime_error {
private:
  std::vector<TraceInfo> trace_stack_;
  std::string original_message_;
  mutable std::string full_message_;

public:
  TracedException(const std::string &message)
      : std::runtime_error(message), original_message_(message) {}

  void add_trace(const TraceInfo &trace_point) {
    trace_stack_.push_back(trace_point);
    full_message_.clear();
  }

  const std::vector<TraceInfo> &get_trace_stack() const { return trace_stack_; }

  const std::string &get_original_message() const { return original_message_; }

  const char *what() const noexcept override {
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
      oss << "TUnit trace (most recent first):\n";
      for (auto it = trace_stack_.rbegin(); it != trace_stack_.rend(); ++it) {
        oss << it->to_string() << "\n";
      }
    }
    full_message_ = oss.str();
  }
};

class TraceContext {
private:
  static thread_local std::vector<TraceInfo> current_trace_;

public:
  static void push_trace(const std::string &file, std::int32_t line,
                         const std::string &message) {
    current_trace_.emplace_back(file, line, message);
  }

  static void pop_trace() {
    if (!current_trace_.empty()) {
      current_trace_.pop_back();
    }
  }

  static std::vector<TraceInfo> get_current_trace() { return current_trace_; }

  static void enrich_exception(TracedException &ex) {
    for (const auto &trace_point : current_trace_) {
      ex.add_trace(trace_point);
    }
  }
  static std::string get_trace_output() {
    if (current_trace_.empty()) {
      return "";
    }

    std::ostringstream oss;
    oss << "TUnit trace:\n";
    for (auto it = current_trace_.rbegin(); it != current_trace_.rend(); ++it) {
      oss << it->to_string() << "\n";
    }
    return oss.str();
  }
};

class ScopedTrace {
private:
  std::string file_;
  std::int32_t line_;
  std::string message_;

public:
  ScopedTrace(const std::string &file, std::int32_t line,
              const std::string &message)
      : file_(file), line_(line), message_(message) {
    TraceContext::push_trace(file_, line_, message_); // RAII
  }

  ~ScopedTrace() { TraceContext::pop_trace(); }

  const std::string &file() const { return file_; }
  std::int32_t line() const { return line_; }
  const std::string &message() const { return message_; }

  std::string to_string() const {
    std::ostringstream oss;
    if (!file_.empty()) {
      oss << file_ << ":" << line_ << ": " << message_;
    } else {
      oss << message_;
    }
    return oss.str();
  }

  TraceInfo to_trace_info() const { return TraceInfo(file_, line_, message_); }

  // Non-copyable/movable for safety
  ScopedTrace(const ScopedTrace &) = delete;
  ScopedTrace &operator=(const ScopedTrace &) = delete;
  ScopedTrace(ScopedTrace &&) = delete;
  ScopedTrace &operator=(ScopedTrace &&) = delete;
};

// Automatic enrichment
template <typename... Args> [[noreturn]] void throw_traced(Args &&...args) {
  try {
    TracedException ex(std::forward<Args>(args)...);
    TraceContext::enrich_exception(ex);
    throw ex;
  } catch (TracedException &ex) {
    // Only enrich if there is a parent scope above (i.e., current_trace_ is not
    // empty)
    if (!TraceContext::get_current_trace().empty()) {
      TraceContext::enrich_exception(ex);
    }
    printf("TracedException: %s\n", ex.what());
    exit(EXIT_SUCCESS); // program will terminate here
    throw;
  }
}

} // namespace trace
} // namespace tunit
