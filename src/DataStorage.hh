#ifndef STATSD_DATA_STORAGE_HH
#define STATSD_DATA_STORAGE_HH

#include <chrono>
#include <memory>
#include <thread>
#include <condition_variable>

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

   void dump(std::chrono::system_clock::time_point const ts_begin,
             std::chrono::system_clock::time_point const ts_end) {

      std::chrono::nanoseconds const diff(ts_end-ts_begin);
      std::cout << "DIFF: "
               << std::chrono::duration_cast<std::chrono::nanoseconds>(
                  diff).count() << std::endl;

      std::cout
         << std::chrono::duration_cast<std::chrono::nanoseconds>(
            ts_begin.time_since_epoch()).count()
         << ":"
         << std::chrono::duration_cast<std::chrono::nanoseconds>(
            ts_end.time_since_epoch()).count()
         << "{" << std::endl;

      for( auto const counter : m_counters ) {
         std::cout << counter.first << ": " << counter.second << std::endl;
      }

      std::cout << "}" << std::endl;
   }

private:
   std::map<std::string, double> m_counters;
};

class raw_storage {
public:

   raw_storage()
      : m_time_begin(std::chrono::high_resolution_clock::now()) {
   }

   std::chrono::system_clock::time_point const &
   get_time_begin() const {
      return m_time_begin;
   }

   void counter_increment(std::string const & name, double const inc) {
      m_counters.increment(name, inc);
   }

   void dump(std::chrono::system_clock::time_point const ts_end) {
      m_counters.dump(m_time_begin, ts_end);
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
        m_dump_thread(&data_storage::run, this),
        m_terminate(false) {
   }

   void counter_increment(std::string const & name, double const inc) {
      m_current_raw_storage->counter_increment(name, inc);
   }

   void join() {
      {
         std::unique_lock<std::mutex> lk(m_terminate_mutex);
         m_terminate = true;
         m_terminate_cond.notify_one();
      }

      m_dump_thread.join();
   }

private:
   void dump() {
      std::shared_ptr<raw_storage> d_storage(
         std::make_shared<raw_storage>());
      // ToDo: Is this really thread safe?
      m_current_raw_storage.swap(d_storage);
      d_storage->dump(m_current_raw_storage->get_time_begin());

#if 0
      std::chrono::system_clock::time_point now(
         std::chrono::high_resolution_clock::now());
#endif
   }

   void run() {
      while(true) {
         bool should_terminate(false);

         {
            std::unique_lock<std::mutex> lk(m_terminate_mutex);
            m_terminate_cond.wait_for(lk, m_wait_interval);
            should_terminate = m_terminate;
         }

         dump();

         if(should_terminate) {
            break;
         }
      }
   }

   std::chrono::milliseconds const m_wait_interval;
   raw_storage_sp m_current_raw_storage;
   std::thread m_dump_thread;

   std::mutex              m_terminate_mutex;
   std::condition_variable m_terminate_cond;
   bool                    m_terminate;
};

}

#endif
