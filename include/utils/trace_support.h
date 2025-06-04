/**
 * Exception tracing system for detailed error context and debugging
 */
#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace tUnit
{
namespace trace
{

#define TUNIT_SCOPED_TRACE(msg) \
  tUnit::trace::ScopedTrace _scoped_trace_##__LINE__(__FILE__, __LINE__, (msg))

#define TUNIT_TRACE_FUNCTION() TUNIT_SCOPED_TRACE(__func__)

struct TraceInfo
{
  std::string file_;
  std::int32_t line_;
  std::string msg_;

  TraceInfo(const std::string &file, std::int32_t line, const std::string &msg) : file_(file), line_(line), msg_(msg) {}

  std::string to_string() const
  {
    std::ostringstream oss;
    if (!file_.empty())
    {
      oss << file_ << ":" << line_ << ": " << msg_;
    }
    else
    {
      oss << msg_;
    }
    return oss.str();
  }
};

class TracedException : public std::runtime_error
{
private:
  std::vector<TraceInfo> trace_stack_;
  std::string original_message_;
  mutable std::string full_message_;

public:
  TracedException(const std::string &message) : std::runtime_error(message), original_message_(message) {}

  void add_trace(const TraceInfo &trace_point)
  {
    trace_stack_.push_back(trace_point);
    full_message_.clear();
  }

  const std::vector<TraceInfo> &get_trace_stack() const { return trace_stack_; }

  const std::string &get_original_message() const { return original_message_; }

  const char *what() const noexcept override
  {
    if (full_message_.empty())
    {
      update_message();
    }
    return full_message_.c_str();
  }

private:
  void update_message() const
  {
    std::ostringstream oss;
    oss << "Error: " << original_message_ << "\n";
    if (!trace_stack_.empty())
    {
      oss << "TUnit trace (most recent first):\n";
      for (auto it = trace_stack_.rbegin(); it != trace_stack_.rend(); ++it)
      {
        oss << it->to_string() << "\n";
      }
    }
    full_message_ = oss.str();
  }
};

class TraceContext
{
public:
  static void push_trace(const std::string &file, std::int32_t line, const std::string &message)
  {
    current_trace_.emplace_back(file, line, message);
  }

  static void pop_trace()
  {
    if (!current_trace_.empty())
    {
      current_trace_.pop_back();
    }
  }

  static std::vector<TraceInfo> get_current_trace() { return current_trace_; }

  static void enrich_exception(TracedException &ex)
  {
    for (const auto &trace_point : current_trace_)
    {
      ex.add_trace(trace_point);
    }
  }
  static std::string get_trace_output()
  {
    if (current_trace_.empty())
    {
      return "";
    }

    std::ostringstream oss;
    oss << "TUnit trace:\n";
    for (auto it = current_trace_.rbegin(); it != current_trace_.rend(); ++it)
    {
      oss << it->to_string() << "\n";
    }
    return oss.str();
  }

private:
  static thread_local std::vector<TraceInfo> current_trace_;
};

class ScopedTrace
{
private:
  std::string file_;
  std::int32_t line_;
  std::string message_;

public:
  ScopedTrace(const std::string &file, std::int32_t line, const std::string &message) : file_(file), line_(line), message_(message)
  {
    TraceContext::push_trace(file_, line_, message_); // RAII
  }

  ~ScopedTrace() { TraceContext::pop_trace(); }

  const std::string &file() const { return file_; }
  std::int32_t line() const { return line_; }
  const std::string &message() const { return message_; }

  std::string to_string() const
  {
    std::ostringstream oss;
    if (!file_.empty())
    {
      oss << file_ << ":" << line_ << ": " << message_;
    }
    else
    {
      oss << message_;
    }
    return oss.str();
  }

  TraceInfo to_trace_info() const { return TraceInfo(file_, line_, message_); }

  // Non-copyable/movable
  ScopedTrace(const ScopedTrace &) = delete;
  ScopedTrace &operator=(const ScopedTrace &) = delete;
  ScopedTrace(ScopedTrace &&) = delete;
  ScopedTrace &operator=(ScopedTrace &&) = delete;
};

// Automatic enrichment
template <typename... Args>
[[noreturn]] void throw_traced(Args &&...args)
{
  TracedException ex(std::forward<Args>(args)...);
  TraceContext::enrich_exception(ex);
  throw ex;
}

} // namespace trace
} // namespace tUnit

/*
 * ===============================================================================
 * TRACE SYSTEM USAGE GUIDE
 * ===============================================================================
 *
 * This trace system provides automatic call stack tracking for debugging test
 * failures and framework errors. When exceptions are thrown using throw_traced(),
 * they automatically include a trace of where the error occurred.
 *
 * ===============================================================================
 * WHEN TO USE EACH
 * ===============================================================================
 *
 * TUNIT_TRACE_FUNCTION():
 * - Use at the beginning of any function you want to track
 * - Automatically uses the function name as the trace message
 * - Essential for predicates, test functions, and complex framework methods
 *
 * TUNIT_SCOPED_TRACE(message):
 * - Use for custom trace points within a function
 * - Great for marking different phases of complex operations
 * - Use descriptive messages like "validation_phase" or "error_handling"
 *
 * tUnit::trace::throw_traced():
 * - Use instead of regular exceptions to include trace information
 * - Automatically captures the current trace context
 * - Provides a detailed error message with the full trace stack
 * - Use for any error condition in predicates, test cases, or framework logic
 */