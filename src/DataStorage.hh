#ifndef STATSD_DATA_STORAGE_HH
#define STATSD_DATA_STORAGE_HH

#include <chrono>
#include <memory>
#include <atomic>
#include <thread>

namespace statsd {

class raw_counters {
};

class raw_storage {
public:
   void dump(std::chrono::system_clock::time_point const
             beginning_of_interval);

   std::chrono::system_clock::time_point const &
   get_time_begin() const {
      return m_time_begin;
   }

private:
   std::chrono::system_clock::time_point const m_time_begin;
   raw_counters m_counters;
};

using raw_storage_sp = std::shared_ptr<raw_storage>;
using raw_storage_asp = std::atomic<raw_storage_sp>;

/**
 * data_storage keeps track about the data of one
 * interval.
 */
class data_storage {
public:
   data_storage(std::chrono::milliseconds const wait_interval =
                std::chrono::milliseconds(10000));

   void counter_increment(std::string const & name, double const & inc);

   void join();

private:
   void dump() {
      std::shared_ptr<raw_storage> new_storage(
         std::make_shared<raw_storage>());
      raw_storage_sp old_storage(
         std::atomic_exchange(&m_current_raw_storage,
                              new_storage));
      old_storage->dump(new_storage->get_time_begin());

#if 0
      std::chrono::system_clock::time_point now(
         std::chrono::high_resolution_clock::now());
#endif
   }

   void run() {
      while(true) {
         std::this_thread::sleep_for(m_wait_interval);
         dump();
      }
   }

   std::chrono::milliseconds const m_wait_interval;

   raw_storage_asp m_current_raw_storage;
};

}

#endif
