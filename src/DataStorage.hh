#ifndef STATSD_DATA_STORAGE_HH
#define STATSD_DATA_STORAGE_HH

#include <chrono>
#include <memory>
#include <thread>

namespace statsd {

class raw_counters {
public:
   void increment(std::string const & name, double const inc) {
      auto const g(m_counters.find(name));
      if(g==m_counters.end()) {
         m_counters.insert(std::make_pair(name, inc));
      } else {
         g->second += inc;
      }
   }

   void dump(std::chrono::system_clock::time_point const
             /* beginning_of_interval */) {
      for( auto const counter : m_counters ) {
         std::cout << counter.first << ": " << counter.second << std::endl;
      }
   }

private:
   std::map<std::string, double> m_counters;
};

class raw_storage {
public:

   std::chrono::system_clock::time_point const &
   get_time_begin() const {
      return m_time_begin;
   }

   void counter_increment(std::string const & name, double const inc) {
      m_counters.increment(name, inc);
   }

   void dump(std::chrono::system_clock::time_point const
             beginning_of_interval) {
      m_counters.dump(beginning_of_interval);
   }

private:
   std::chrono::system_clock::time_point const m_time_begin;
   raw_counters m_counters;
};

using raw_storage_sp = std::shared_ptr<raw_storage>;

/**
 * data_storage keeps track about the data of one
 * interval.
 */
class data_storage {
public:
   data_storage(std::chrono::milliseconds const wait_interval =
                std::chrono::milliseconds(10000))
      : m_wait_interval(wait_interval),
        m_current_raw_storage(std::make_shared<raw_storage>()),
        m_dump_thread(&data_storage::run, this) {
   }

   void counter_increment(std::string const & name, double const inc) {
      m_current_raw_storage->counter_increment(name, inc);
   }

   void join() {
      // ToDo: Signal / Join thread
      m_dump_thread.join();
   }

private:
   void dump() {
      std::shared_ptr<raw_storage> new_storage(
         std::make_shared<raw_storage>());
      // ToDo: Is this really thread safe?
      m_current_raw_storage.swap(new_storage);
      new_storage->dump(new_storage->get_time_begin());

#if 0
      std::chrono::system_clock::time_point now(
         std::chrono::high_resolution_clock::now());
#endif
   }

   void run() {
//      while(true) {
      {
         std::this_thread::sleep_for(m_wait_interval);
         dump();
      }
   }

   std::chrono::milliseconds const m_wait_interval;
   raw_storage_sp m_current_raw_storage;
   std::thread m_dump_thread;
};

}

#endif
