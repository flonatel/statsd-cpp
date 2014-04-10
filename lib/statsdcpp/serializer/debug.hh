#ifndef STATSD_CPP_SERIALIZER_DEBUG_HH
#define STATSD_CPP_SERIALIZER_DEBUG_HH

#include <chrono>
#include <sstream>
#include <statsdcpp/counter.hh>

namespace statsdcpp { namespace serializer {

class debug {
public:
   template< typename TWriter >
   static void write(
      TWriter & writer,
      std::chrono::system_clock::time_point const & begin_ts,
      std::chrono::system_clock::time_point const & end_ts,
      statsdcpp::base< TWriter> const * base) {
      base->serialize_debug(writer, begin_ts, end_ts);
   }

   template< typename TWriter >
   static void write(
      TWriter & writer,
      std::chrono::system_clock::time_point const & begin_ts,
      std::chrono::system_clock::time_point const & end_ts,
      statsdcpp::counter const & counter) {
      counter.serialize_debug(writer, begin_ts, end_ts);
   }

   template< typename TSerializer, typename TWriter >
   static void begin(
      TWriter & writer,
      statsdcpp::collector<TSerializer, TWriter> const & clcrt) {
      clcrt.serialize_debug_begin(writer);
   }

   template< typename TSerializer, typename TWriter >
   static void end(
      TWriter & writer,
      statsdcpp::collector<TSerializer, TWriter> const & clcrt) {
      clcrt.serialize_debug_end(writer);
   }

};

}}

#endif
