/*
 * Checks the performance of the collector class.
 *
 * It can be easily configured to run with and without
 * the collector.
 */

#define USE_STATSD_CORRELATOR

#include <ctime>
#include <thread>
#include <iostream>
#include <ptl/object_pool.hh>
#include <statsdcpp/correlator.hh>
#include <statsdcpp/output/fd.hh>
#include <statsdcpp/serializer/carbon.hh>
#include <statsdcpp/collector.hh>

template< typename OBJ_TYPE >
using Queue = ptl::object_pool::pool<
   OBJ_TYPE,
   ptl::object_pool::policies::threading::multi,
   ptl::object_pool::policies::notify::all,
   ptl::object_pool::policies::notify::all,
   ptl::object_pool::policies::termination::terminatable,
   ptl::object_pool::policies::container::queue,
   ptl::object_pool::policies::size_handling::constant >;

using QTermEx = ptl::object_pool::terminate_except;

constexpr long queue_size(1000);

enum class checkpoint {
   cp_in, cp_2, cp_3, cp_5, cp_7, cp_11, cp_13, cp_17, cp_23, cp_out };

using corr_t = statsdcpp::correlator_sp<
   statsdcpp::output::fd, int, checkpoint > ;

void f_in(corr_t & corr, Queue< long > & q_in) {
   (void)corr;
   for(long i=1; i<1000000; ++i) {
#ifdef USE_STATSD_CORRELATOR
      corr->in(i, checkpoint::cp_in);
#endif
      q_in.push(i);
   }
   q_in.push(0);
}

void f_process(corr_t & corr, long n, checkpoint cp, Queue< long > & q_in,
   Queue< long > & q_a, Queue< long > & q_b) {
   (void)corr;
   struct timespec tspec {0, 10*n};

   while(true) {
      long const c(q_in.pop());

//      std::cerr << n << ": " << c << std::endl;

      if(c==0) {
         q_a.push(0);
         q_b.push(0);
         return;
      }

#ifdef USE_STATSD_CORRELATOR
      corr->seen(c, cp);
#endif

      if(c%n==0) {
         q_a.push(c);
      } else {
         q_b.push(c);
      }

      nanosleep(&tspec, NULL);
   }
}

void f_out(corr_t & corr, Queue< long > & q_out) {
   (void)corr;
   int c;
   while( (c = q_out.pop())!=0) {
#ifdef USE_STATSD_CORRELATOR
      corr->out(c, checkpoint::cp_out);
#endif
   }
}

std::atomic_int continue_to_run(1);

//void corr_thread(

int main() {
   statsdcpp::output::fd fd_writer(1);
   statsdcpp::collector< statsdcpp::serializer::carbon,
                         statsdcpp::output::fd > collector(fd_writer);

   std::vector< statsdcpp::transition<checkpoint> > const tr {
      { checkpoint::cp_in, checkpoint::cp_2, "IN->2" },
      { checkpoint::cp_2, checkpoint::cp_3, "2->3" },
      { checkpoint::cp_2, checkpoint::cp_out, "2->OUT" },
      { checkpoint::cp_3, checkpoint::cp_5, "3->5" },
      { checkpoint::cp_3, checkpoint::cp_out, "3->OUT" },
      { checkpoint::cp_5, checkpoint::cp_7, "5->7" },
      { checkpoint::cp_5, checkpoint::cp_out, "5->OUT" },
      { checkpoint::cp_7, checkpoint::cp_11, "7->11" },
      { checkpoint::cp_7, checkpoint::cp_out, "7->OUT" },
      { checkpoint::cp_11, checkpoint::cp_13, "11->13" },
      { checkpoint::cp_11, checkpoint::cp_out, "11->OUT" },
      { checkpoint::cp_13, checkpoint::cp_17, "13->17" },
      { checkpoint::cp_13, checkpoint::cp_out, "13->OUT" },
      { checkpoint::cp_17, checkpoint::cp_23, "17->23" },
      { checkpoint::cp_17, checkpoint::cp_out, "17->OUT" },
      { checkpoint::cp_23, checkpoint::cp_out, "23->OUT" }
   };

   corr_t crltr(
      collector.generate_correlator<int, checkpoint>(
         "org.flonatel.statsd.test.perf", tr));

   Queue< long >  queue_in(queue_size);
   Queue< long >  queue_out(queue_size);
   Queue< long >  queue_proc1(queue_size);
   Queue< long >  queue_proc2(queue_size);
   Queue< long >  queue_proc3(queue_size);
   Queue< long >  queue_proc4(queue_size);
   Queue< long >  queue_proc5(queue_size);
   Queue< long >  queue_proc6(queue_size);
   Queue< long >  queue_proc7(queue_size);
   Queue< long >  queue_proc8(queue_size);

   std::thread thr_in(f_in, std::ref(crltr), std::ref(queue_in));
   std::thread thr_p1(f_process, std::ref(crltr), 2, checkpoint::cp_2,
                      std::ref(queue_in),
                      std::ref(queue_out), std::ref(queue_proc1));
   std::thread thr_p2(f_process, std::ref(crltr), 3, checkpoint::cp_3,
                      std::ref(queue_proc1),
                      std::ref(queue_out), std::ref(queue_proc2));
   std::thread thr_p3(f_process, std::ref(crltr), 5, checkpoint::cp_5,
                      std::ref(queue_proc2),
                      std::ref(queue_out), std::ref(queue_proc3));
   std::thread thr_p4(f_process, std::ref(crltr), 7, checkpoint::cp_7,
                      std::ref(queue_proc3),
                      std::ref(queue_out), std::ref(queue_proc4));
   std::thread thr_p5(f_process, std::ref(crltr), 11, checkpoint::cp_11,
                      std::ref(queue_proc4),
                      std::ref(queue_out), std::ref(queue_proc5));
   std::thread thr_p6(f_process, std::ref(crltr), 13, checkpoint::cp_13,
                      std::ref(queue_proc5),
                      std::ref(queue_out), std::ref(queue_proc6));
   std::thread thr_p7(f_process, std::ref(crltr), 17, checkpoint::cp_17,
                      std::ref(queue_proc6),
                      std::ref(queue_out), std::ref(queue_proc7));
   std::thread thr_p8(f_process, std::ref(crltr), 23, checkpoint::cp_23,
                      std::ref(queue_proc7),
                      std::ref(queue_out), std::ref(queue_proc8));
   std::thread thr_out1(f_out, std::ref(crltr), std::ref(queue_out));
   std::thread thr_out2(f_out, std::ref(crltr), std::ref(queue_proc8));

   thr_in.join();
   thr_p1.join();
   thr_p2.join();
   thr_p3.join();
   thr_p4.join();
   thr_p5.join();
   thr_p6.join();
   thr_p7.join();
   thr_p8.join();
   thr_out1.join();
   thr_out2.join();

   collector.flush();

   return 0;
}
