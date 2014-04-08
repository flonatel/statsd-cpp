#ifndef STATSD_CPP_COUNTER_HH
#define STATSD_CPP_COUNTER_HH

#include <string>
#include <atomic>
#include <memory>

namespace statsdcpp {

class collector;

class counter {
public:
   void inc() { ++_cnt; }

   counter() = delete;
   counter(std::string const & name);

private:
   std::string const _name;
   std::atomic_uint_fast64_t _cnt;
};

using counter_sp = std::shared_ptr<counter>;

}

#endif
