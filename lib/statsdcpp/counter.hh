#ifndef STATSD_CPP_COUNTER_HH
#define STATSD_CPP_COUNTER_HH

#include <string>
#include <atomic>
#include <memory>

namespace statsdcpp {

class counter {
public:
   void inc() { ++_cnt; }

   counter() = delete;
   counter(std::string const & name);

   std::string const & name() const { return _name; }
   uint64_t get_and_reset() { return _cnt.exchange(0); }

private:
   std::string const _name;
   std::atomic_uint_fast64_t _cnt;
};

using counter_sp = std::shared_ptr<counter>;

}

#endif
