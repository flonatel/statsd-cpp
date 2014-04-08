#ifndef STATSD_CPP_COUNTER_HH
#define STATSD_CPP_COUNTER_HH

#include <string>
#include <atomic>

namespace statsdcpp {

class counter {
public:
   counter(std::string const & name);

   void operator++() { ++_cnt; }

private:
   std::string const _name;
   std::atomic_uint_fast64_t _cnt;
};

}

#endif
