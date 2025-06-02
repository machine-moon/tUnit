#include "../../include/utils/trace_support.h"

namespace tunit {
namespace trace {

// Definition of the thread_local static member
thread_local std::vector<TraceInfo> TraceContext::current_trace_;

}  // namespace trace
}  // namespace tunit
