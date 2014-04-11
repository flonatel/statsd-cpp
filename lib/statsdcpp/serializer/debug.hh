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
                     statsdcpp::base< TWriter> * base) {
      base->serialize_debug(writer);
   }
};

}}

#endif
