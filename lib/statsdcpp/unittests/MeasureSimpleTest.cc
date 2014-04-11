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
   time_measure->add(40.0_ms);
   time_measure->add(30.0_ms);

   collector.flush();

   ASSERT_EQ("Measure:org.flonatel.statsd.test.msur1:0.02:0.04:0.03:3\n",
             string_writer.str());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
