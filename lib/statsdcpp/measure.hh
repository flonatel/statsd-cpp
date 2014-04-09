#ifndef STATSD_CPP_MEASURE_HH
#define STATSD_CPP_MEASURE_HH

#include <statsdcpp/quantity.hh>

namespace statsdcpp {

// This is a generalization of the original statsd timer and gauge.
// It has the funtionality of timer but for arbritrary measurements.
template< typename TQuantity >
class measure {
public:
   measure(std::string const & name);
};

}

#endif
