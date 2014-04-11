#ifndef STATSDCPP_TIMEUTILS_HH
#define STATSDCPP_TIMEUTILS_HH

#include <chrono>

namespace statsdcpp { namespace timeutils {

std::chrono::system_clock::time_point
inline interval_middle(
   std::chrono::system_clock::time_point const & begin_ts,
   std::chrono::system_clock::time_point const & end_ts) {
   std::chrono::nanoseconds const diff(end_ts-begin_ts);
   std::chrono::nanoseconds half_diff(diff);
   half_diff /= 2;
   return begin_ts + half_diff;
}

}}

#endif
