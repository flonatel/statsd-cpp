#ifndef STATSD_CPP_SERIALIZER_DEBUG_HH
#define STATSD_CPP_SERIALIZER_DEBUG_HH

#include <chrono>
#include <sstream>
#include <statsdcpp/counter.hh>

namespace statsdcpp { namespace serializer {

class debug {
public:
   template< typename TWriter >
   static void write(TWriter & writer,
                     statsdcpp::counter const & counter) {
      counter.serialize_debug(writer);
   }
};

// *** Implementation
#if 0

template< typename TWriter >
debug< TWriter >::debug(TWriter & writer)
: _writer(writer) {
}

template< typename TWriter >
void debug< TWriter >::bulk_info(
   std::chrono::system_clock::time_point /* ts_begin */,
   std::chrono::system_clock::time_point /* ts_end */,
   uint64_t /* counter_cnt */) {

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
}
#endif

#if 0
template< typename TWriter >
void debug< TWriter >::counter(std::string const & name,
                               uint64_t const cnt) {
   std::ostringstream ostr;
   ostr << "Counter:" << name << ":" << cnt << std::endl;
   _writer.write(ostr.str().c_str(), ostr.str().size());
}
#endif

}}

#endif
