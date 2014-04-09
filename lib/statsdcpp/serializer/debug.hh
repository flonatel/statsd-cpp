#ifndef STATSD_CPP_SERIALIZER_DEBUG_HH
#define STATSD_CPP_SERIALIZER_DEBUG_HH

#include <chrono>
#include <sstream>

namespace statsdcpp { namespace serializer {

template< typename TWriter >
class debug {
public:
   debug(TWriter & writer);

   void bulk_info(
      std::chrono::system_clock::time_point begin,
      std::chrono::system_clock::time_point end,
      uint64_t counter_cnt);

   void counter(std::string const & name,
                uint64_t const cnt);

private:
   TWriter & _writer;
};

// *** Implementation

template< typename TWriter >
debug< TWriter >::debug(TWriter & writer)
: _writer(writer) {
}

template< typename TWriter >
void debug< TWriter >::bulk_info(
   std::chrono::system_clock::time_point /* ts_begin */,
   std::chrono::system_clock::time_point /* ts_end */,
   uint64_t /* counter_cnt */) {

#if 0
   std::ostringstream ostr;

   // Timepoint does not matter for debug output
   ostr << "TimeIntervalInNS:"
        << std::chrono::duration_cast<std::chrono::nanoseconds>(
           ts_end.time_since_epoch()).count()
      - std::chrono::duration_cast<std::chrono::nanoseconds>(
         ts_begin.time_since_epoch()).count()
        << ";"
        << "CounterCnt:" << counter_cnt << std::endl;

   _writer.write(ostr.str().c_str(), ostr.str().size());
#endif
}

template< typename TWriter >
void debug< TWriter >::counter(std::string const & name,
                               uint64_t const cnt) {
   std::ostringstream ostr;
   ostr << "Counter:" << name << ":" << cnt << std::endl;
   _writer.write(ostr.str().c_str(), ostr.str().size());
}

}}

#endif
