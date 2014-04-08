#include <gtest/gtest.h>
#include <statsdcpp/counter.hh>
#include <statsdcpp/collector.hh>

class CounterSimple : public ::testing::Test {
};

TEST_F(CounterSimple, test_one_counter) {
   statsdcpp::collector collector;
   statsdcpp::counter_sp cnt(
      collector.generate_counter("org.flonatel.statsd.test.cnt1"));
   cnt->inc();
   // Because it is not possible to read counters, there
   // is no simple way of checking for the right result.
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
