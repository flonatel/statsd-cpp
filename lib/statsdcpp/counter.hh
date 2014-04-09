#ifndef STATSD_CPP_COUNTER_HH
#define STATSD_CPP_COUNTER_HH

#include <chrono>
#include <string>
#include <atomic>
#include <memory>
#include <sstream>
#include <iostream>

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
   void serialize_debug(
      TWriter & writer,
      std::chrono::system_clock::time_point const & begin_ts,
      std::chrono::system_clock::time_point const & end_ts) const;

   template< typename TWriter >
   void serialize_carbon(
      TWriter & writer,
      std::chrono::system_clock::time_point const & begin_ts,
      std::chrono::system_clock::time_point const & end_ts) const;

private:
   std::string const _name;
   std::atomic_uint_fast64_t _cnt;
};

using counter_sp = std::shared_ptr<counter>;


template< typename TWriter >
void counter::serialize_debug(
   TWriter & writer,
   std::chrono::system_clock::time_point const & /* begin_ts */,
   std::chrono::system_clock::time_point const & /* end_ts */) const {
   std::ostringstream ostr;
   ostr << "Counter:" << _name << ":" << _cnt << std::endl;
   writer.write(ostr.str().c_str(), ostr.str().size());
}

template< typename TWriter >
void counter::serialize_carbon(
   TWriter & writer,
   std::chrono::system_clock::time_point const & begin_ts,
   std::chrono::system_clock::time_point const & end_ts) const {

   std::chrono::nanoseconds const diff(end_ts-begin_ts);
   std::chrono::nanoseconds half_diff(diff);
   half_diff /= 2;
   std::chrono::system_clock::time_point const mid(begin_ts + half_diff);

   double const cnt_per_nsec(
      static_cast<double>(_cnt)
      / std::chrono::duration_cast<std::chrono::nanoseconds>(
         diff).count());
   double const cnt_per_sec(cnt_per_nsec * 1000000000);

   std::ostringstream ostr;
   ostr << _name << " " << cnt_per_sec << " "
        << std::chrono::duration_cast<std::chrono::seconds>(
           mid.time_since_epoch()).count() << std::endl;
   writer.write(ostr.str().c_str(), ostr.str().size());
}

}

#endif
