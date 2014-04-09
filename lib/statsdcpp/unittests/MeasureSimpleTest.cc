#include <gtest/gtest.h>
#include <statsdcpp/measure.hh>

class MeasureSimple : public ::testing::Test {
public:
};

TEST_F(MeasureSimple, test_basic_computation) {
   using namespace statsdcpp::siunits;

   statsdcpp::measure<statsdcpp::siunits::time> time_measure(
      "org.flonatel.statsd.test.msur1");

   time_measure.add(20.0_ms);

   ASSERT_EQ(1, time_measure.count_values());
   ASSERT_EQ(20.0_ms, time_measure.average());
   ASSERT_EQ(20.0_ms, time_measure.max());
   ASSERT_EQ(20.0_ms, time_measure.min());

   time_measure.add(40.0_ms);

   ASSERT_EQ(2, time_measure.count_values());
   ASSERT_EQ(30.0_ms, time_measure.average());
   ASSERT_EQ(40.0_ms, time_measure.max());
   ASSERT_EQ(20.0_ms, time_measure.min());

   time_measure.add(30.0_ms);

   ASSERT_EQ(3, time_measure.count_values());
   ASSERT_EQ(30.0_ms, time_measure.average());
   ASSERT_EQ(40.0_ms, time_measure.max());
   ASSERT_EQ(20.0_ms, time_measure.min());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
