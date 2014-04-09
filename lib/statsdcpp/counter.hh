#ifndef STATSD_CPP_COUNTER_HH
#define STATSD_CPP_COUNTER_HH

#include <string>
#include <atomic>
#include <memory>
#include <sstream>

namespace statsdcpp {

class counter {
public:
   void inc() { ++_cnt; }
   void inc(uint64_t const n) { _cnt+=n; }

   counter() = delete;
   counter(std::string const & name);

   std::string const & name() const { return _name; }
   uint64_t get_and_reset() { return _cnt.exchange(0); }

   template< typename TWriter >
   void serialize_debug(TWriter & writer) const;

private:
   std::string const _name;
   std::atomic_uint_fast64_t _cnt;
};

using counter_sp = std::shared_ptr<counter>;


template< typename TWriter >
void counter::serialize_debug(TWriter & writer) const {
   std::ostringstream ostr;
   ostr << "Counter:" << _name << ":" << _cnt << std::endl;
   writer.write(ostr.str().c_str(), ostr.str().size());
}

}

#endif
