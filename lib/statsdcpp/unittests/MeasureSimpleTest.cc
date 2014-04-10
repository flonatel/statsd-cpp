#include <gtest/gtest.h>
#include <statsdcpp/measure.hh>
#include <statsdcpp/collector.hh>
#include <statsdcpp/output/string.hh>
#include <statsdcpp/serializer/debug.hh>

class MeasureSimple : public ::testing::Test {
public:
   MeasureSimple()
      : collector(string_writer) {
   }

   statsdcpp::output::string string_writer;
   statsdcpp::serializer::debug debug_serializer;
   statsdcpp::collector< statsdcpp::serializer::debug,
                         statsdcpp::output::string > collector;
};

TEST_F(MeasureSimple, test_basic_computation) {
   using namespace statsdcpp::siunits;

   statsdcpp::measure_sp< statsdcpp::siunits::time,
                          statsdcpp::output::string >
      time_measure(
         collector.generate_measure<statsdcpp::siunits::time>(
            "org.flonatel.statsd.test.msur1"));

   time_measure->add(20.0_ms);

   ASSERT_EQ(1, time_measure->count_values());
   ASSERT_EQ(20.0_ms, time_measure->average());
   ASSERT_EQ(20.0_ms, time_measure->max());
   ASSERT_EQ(20.0_ms, time_measure->min());

   time_measure->add(40.0_ms);

   ASSERT_EQ(2, time_measure->count_values());
   ASSERT_EQ(30.0_ms, time_measure->average());
   ASSERT_EQ(40.0_ms, time_measure->max());
   ASSERT_EQ(20.0_ms, time_measure->min());

   time_measure->add(30.0_ms);

   ASSERT_EQ(3, time_measure->count_values());
   ASSERT_EQ(30.0_ms, time_measure->average());
   ASSERT_EQ(40.0_ms, time_measure->max());
   ASSERT_EQ(20.0_ms, time_measure->min());

   collector.flush();

   ASSERT_EQ("Counter:org.flonatel.statsd.test.cnt1:1\n",
             string_writer.str());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
