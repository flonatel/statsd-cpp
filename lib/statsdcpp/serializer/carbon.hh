#ifndef STATSD_CPP_SERIALIZER_CARBON_HH
#define STATSD_CPP_SERIALIZER_CARBON_HH

#include <chrono>
#include <sstream>
#include <statsdcpp/counter.hh>

namespace statsdcpp { namespace serializer {

class carbon {
public:
   template< typename TWriter >
   static void write(TWriter & writer,
      statsdcpp::base< TWriter> * base) {
      base->serialize_carbon(writer);
   }
};

}}

#endif
