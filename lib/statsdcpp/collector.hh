#ifndef STATSD_COLLECTOR_HH
#define STATSD_COLLECTOR_HH

#include <list>
#include <chrono>
#include <statsdcpp/counter.hh>

namespace statsdcpp {

// The Collector class collects all the data of the different
// types and send them out each interval.

template< typename TSerializer, typename TWriter >
class collector {
public:
   collector(TWriter & writer);

   counter_sp generate_counter(std::string const & name);

   // Force flushing all counters to the writer
   void flush();

private:
   TWriter & _writer;
   std::chrono::system_clock::time_point _begin_ts;
   std::list<counter_sp> _counters;
};

// *** Implementation

template< typename TSerializer, typename TWriter>
collector< TSerializer, TWriter >::collector(TWriter & writer)
: _writer(writer),
  _begin_ts(std::chrono::high_resolution_clock::now()) {
}

template< typename TSerializer, typename TWriter>
counter_sp collector< TSerializer, TWriter >::generate_counter(
   std::string const & name) {
   counter_sp cntr(std::make_shared<counter>(name));
   _counters.push_back(cntr);
   return cntr;
}

template< typename TSerializer, typename TWriter>
void collector< TSerializer, TWriter >::flush() {
   std::chrono::system_clock::time_point const end_ts(
      std::chrono::high_resolution_clock::now());

//   TSerializer::begin(_writer, *this);
   for(auto const & cnt_it : _counters) {
      TSerializer::write(_writer, *(cnt_it.get()));
   }
//   TSerializer::end(_writer, *this);
}

}

#endif
