#ifndef STATSD_COLLECTOR_HH
#define STATSD_COLLECTOR_HH

#include <list>
#include <chrono>
#include <statsdcpp/counter.hh>

namespace statsdcpp {

// The Collector class collects all the data of the different
// types and send them out each interval.

template< typename TSerializer >
class collector {
public:
   collector(TSerializer & serializer);

   counter_sp generate_counter(std::string const & name);

   // Force flushing all counters to the serializer
   void flush();

private:
   TSerializer & _serializer;
   std::chrono::system_clock::time_point _begin_ts;
   std::list<counter_sp> _counters;
};

// *** Implementation

template< typename TSerializer>
collector< TSerializer >::collector(TSerializer & serializer)
: _serializer(serializer),
  _begin_ts(std::chrono::high_resolution_clock::now()) {
}

template< typename TSerializer>
counter_sp collector< TSerializer >::generate_counter(
   std::string const & name) {
   counter_sp cntr(std::make_shared<counter>(name));
   _counters.push_back(cntr);
   return cntr;
}

template< typename TSerializer>
void collector< TSerializer >::flush() {
   std::chrono::system_clock::time_point const end_ts(
      std::chrono::high_resolution_clock::now());

   _serializer.bulk_info(_begin_ts, end_ts, _counters.size());
   for(auto const & cnt_it : _counters) {
      _serializer.counter(cnt_it->name(), cnt_it->get_and_reset());
   }
}

}

#endif
