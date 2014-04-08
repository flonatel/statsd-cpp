#ifndef STATSD_COLLECTOR_HH
#define STATSD_COLLECTOR_HH

#include <map>
#include <statsdcpp/counter.hh>

namespace statsdcpp {

// The Collector class collects all the data of the different
// types and send them out each interval.

class collector {
public:
   counter_sp generate_counter(std::string const & name);

private:
   std::map<std::string, counter_sp> _counters;
};

}

#endif
