#ifndef STATSD_COLLECTOR_HH
#define STATSD_COLLECTOR_HH

#include <list>
#include <chrono>
#include <statsdcpp/counter.hh>
#include <statsdcpp/measure.hh>
#include <statsdcpp/correlator.hh>

namespace statsdcpp {

// The Collector class collects all the data of the different
// types and send them out each interval.

template< typename TSerializer, typename TWriter >
class collector {
public:
   collector(TWriter & writer);

   counter_sp< TWriter > generate_counter(std::string const & name);
   template< typename TQuantity > measure_sp< TQuantity, TWriter >
      generate_measure(std::string const & name);
   template< typename TUID, typename TCheckpoint >
   correlator_sp< TWriter, TUID, TCheckpoint >
   generate_correlator(
      std::string const & name,
      std::vector< statsdcpp::transition< TCheckpoint > > const & iv);

   // Force flushing all counters to the writer
   void flush();

private:
   TWriter & _writer;
   std::list< base_sp< TWriter > > _measurables;
};

// *** Implementation

template< typename TSerializer, typename TWriter>
collector< TSerializer, TWriter >::collector(TWriter & writer)
: _writer(writer) {
}


template< typename TSerializer, typename TWriter>
counter_sp< TWriter >
collector< TSerializer, TWriter >::generate_counter(
   std::string const & name) {
   counter_sp< TWriter > nobj(
      std::make_shared< counter< TWriter > >(name));
   _measurables.push_back(nobj);
   return nobj;
}

template< typename TSerializer, typename TWriter>
template< typename TQuantity >
measure_sp< TQuantity, TWriter >
collector< TSerializer, TWriter >::generate_measure(
   std::string const & name) {
   measure_sp< TQuantity, TWriter > nobj(
      std::make_shared< measure< TQuantity, TWriter > >(name));
   _measurables.push_back(nobj);
   return nobj;
}

template< typename TSerializer, typename TWriter>
template< typename TUID, typename TCheckpoint >
correlator_sp< TWriter, TUID, TCheckpoint >
collector< TSerializer, TWriter >::generate_correlator(
   std::string const & name,
   std::vector< statsdcpp::transition< TCheckpoint > > const & iv) {
   correlator_sp< TWriter, TUID, TCheckpoint > nobj(
      std::make_shared< correlator< TWriter, TUID, TCheckpoint > >(name, iv));
   _measurables.push_back(nobj);
   return nobj;
}

template< typename TSerializer, typename TWriter>
void collector< TSerializer, TWriter >::flush() {
   std::chrono::system_clock::time_point const end_ts(
      std::chrono::high_resolution_clock::now());

   for(auto & msrs_it : _measurables) {
      TSerializer::write(_writer, msrs_it.get());
   }
}

}

#endif
