#include <gtest/gtest.h>
#include <statsd/counter.hh>

class CounterSimple : public ::testing::Test {
};

TEST_F(CounterSimple, test_one_counter) {
   statsd::counter cnt("org.flonatel.statsd.test.cnt1");
   ++cnt;
   // Because it is not possible to read counters, there
   // is no simple way of checking for the right result.
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
