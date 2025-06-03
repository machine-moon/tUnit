#include "utils/trace_support.h"

namespace tUnit
{
namespace trace
{

thread_local std::vector<TraceInfo> TraceContext::current_trace_;

} // namespace trace
} // namespace tUnit
