#ifndef STATSD_CPP_COUNTER_HH
#define STATSD_CPP_COUNTER_HH

#include <chrono>
#include <string>
#include <atomic>
#include <memory>
#include <sstream>
#include <iostream>
#include <statsdcpp/base.hh>
#include <statsdcpp/timeutils.hh>

namespace statsdcpp {

/*
 * counter
 * This is a simple counter implementation.
 * It is optimized for speed: the inc() are atomic operations.
 * inc() is thread-safe.
 * The caller has to ensure that at maximum one of the serialize_ methods
 * is called at the same point of time.
 */
template< typename TWriter >
class counter : public base< TWriter > {
public:
   void inc() { ++_cnt; }
   void inc(uint64_t const n) { _cnt+=n; }

   counter() = delete;
   counter(std::string const & name);

   void serialize_debug(TWriter & writer);
   void serialize_carbon(TWriter & writer);

private:
   uint64_t get_and_reset() { return _cnt.exchange(0); }

   std::atomic_uint_fast64_t _cnt;
};

template< typename TWriter >
using counter_sp = std::shared_ptr< counter< TWriter > >;

template< typename TWriter >
counter< TWriter >::counter(std::string const & name)
   : base< TWriter >(name),
     _cnt(0) {
}

template< typename TWriter >
void counter< TWriter >::serialize_debug(TWriter & writer) {
   std::ostringstream ostr;
   ostr << "Counter:" << this->_name << ":" << get_and_reset() << std::endl;
   writer.write(ostr.str().c_str(), ostr.str().size());
}

template< typename TWriter >
void counter< TWriter >::serialize_carbon(TWriter & writer) {
   std::chrono::system_clock::time_point const end_ts(
      std::chrono::high_resolution_clock::now());
   std::chrono::system_clock::time_point const mid(
      timeutils::interval_middle(this->_begin_ts, end_ts));
   std::chrono::nanoseconds const diff(end_ts - this->_begin_ts);

   double const cnt_per_nsec(
      static_cast<double>(get_and_reset())
      / std::chrono::duration_cast<std::chrono::nanoseconds>(
         diff).count());
   double const cnt_per_sec(cnt_per_nsec * 1000000000);

   std::ostringstream ostr;
   ostr << this->_name << " " << cnt_per_sec << " "
        << std::chrono::duration_cast<std::chrono::seconds>(
           mid.time_since_epoch()).count() << std::endl;
   writer.write(ostr.str().c_str(), ostr.str().size());
   this->_begin_ts = end_ts;
}

}

#endif
