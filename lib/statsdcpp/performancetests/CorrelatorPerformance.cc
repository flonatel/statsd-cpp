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

using corr_t = statsdcpp::correlator_sp<
   statsdcpp::output::fd, int, std::string > ;

void f_in(corr_t & corr, Queue< long > & q_in) {
   std::string const ns("0");
   (void)corr;
   for(long i=1; i<1000000; ++i) {
#ifdef USE_STATSD_CORRELATOR
      corr->in(i, ns);
#endif
      q_in.push(i);
   }
   q_in.push(0);
}

void f_process(corr_t & corr, long n, Queue< long > & q_in,
   Queue< long > & q_a, Queue< long > & q_b) {
   (void)corr;
   std::string const ns(std::to_string(n));

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
      corr->seen(c, ns);
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
   std::string const ns("-1");
   int c;
   while( (c = q_out.pop())!=0) {
#ifdef USE_STATSD_CORRELATOR
      corr->out(c, ns);
#endif
   }
}

std::atomic_int continue_to_run(1);

//void corr_thread(

int main() {
   statsdcpp::output::fd fd_writer(1);
   statsdcpp::collector< statsdcpp::serializer::carbon,
                         statsdcpp::output::fd > collector(fd_writer);

   corr_t crltr(
      collector.generate_correlator<int, std::string>(
         "org.flonatel.statsd.test.perf"));

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
   std::thread thr_p1(f_process, std::ref(crltr), 2, std::ref(queue_in),
                      std::ref(queue_out), std::ref(queue_proc1));
   std::thread thr_p2(f_process, std::ref(crltr), 3, std::ref(queue_proc1),
                      std::ref(queue_out), std::ref(queue_proc2));
   std::thread thr_p3(f_process, std::ref(crltr), 5, std::ref(queue_proc2),
                      std::ref(queue_out), std::ref(queue_proc3));
   std::thread thr_p4(f_process, std::ref(crltr), 7, std::ref(queue_proc3),
                      std::ref(queue_out), std::ref(queue_proc4));
   std::thread thr_p5(f_process, std::ref(crltr), 11, std::ref(queue_proc4),
                      std::ref(queue_out), std::ref(queue_proc5));
   std::thread thr_p6(f_process, std::ref(crltr), 13, std::ref(queue_proc5),
                      std::ref(queue_out), std::ref(queue_proc6));
   std::thread thr_p7(f_process, std::ref(crltr), 17, std::ref(queue_proc6),
                      std::ref(queue_out), std::ref(queue_proc7));
   std::thread thr_p8(f_process, std::ref(crltr), 23, std::ref(queue_proc7),
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
