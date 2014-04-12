/*
 * Checks the performance of the collector class.
 *
 * It can be easily configured to run with and without
 * the collector.
 */

#include <ctime>
#include <thread>
#include <iostream>
#include <ptl/object_pool.hh>

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

void f_in(Queue< long > & q_in) {
   for(long i=1; i<1000000; ++i) {
      q_in.push(i);
   }
   q_in.push(0);
}

void f_process(long n, Queue< long > & q_in,
   Queue< long > & q_a, Queue< long > & q_b) {

   struct timespec tspec {0, 10*n};

   while(true) {
      long const c(q_in.pop());

//      std::cerr << n << ": " << c << std::endl;

      if(c==0) {
         q_a.push(0);
         q_b.push(0);
         return;
      }
      if(c%n==0) {
         q_a.push(c);
      } else {
         q_b.push(c);
      }

      nanosleep(&tspec, NULL);
   }
}

void f_out(Queue< long > & q_out) {
   while(q_out.pop()!=0) {
   }
}

int main() {
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

   std::thread thr_in(f_in, std::ref(queue_in));
   std::thread thr_p1(f_process, 2, std::ref(queue_in),
                      std::ref(queue_out), std::ref(queue_proc1));
   std::thread thr_p2(f_process, 3, std::ref(queue_proc1),
                      std::ref(queue_out), std::ref(queue_proc2));
   std::thread thr_p3(f_process, 5, std::ref(queue_proc2),
                      std::ref(queue_out), std::ref(queue_proc3));
   std::thread thr_p4(f_process, 7, std::ref(queue_proc3),
                      std::ref(queue_out), std::ref(queue_proc4));
   std::thread thr_p5(f_process, 11, std::ref(queue_proc4),
                      std::ref(queue_out), std::ref(queue_proc5));
   std::thread thr_p6(f_process, 13, std::ref(queue_proc5),
                      std::ref(queue_out), std::ref(queue_proc6));
   std::thread thr_p7(f_process, 17, std::ref(queue_proc6),
                      std::ref(queue_out), std::ref(queue_proc7));
   std::thread thr_p8(f_process, 23, std::ref(queue_proc7),
                      std::ref(queue_out), std::ref(queue_proc8));
   std::thread thr_out1(f_out, std::ref(queue_out));
   std::thread thr_out2(f_out, std::ref(queue_proc8));

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

   return 0;
}
