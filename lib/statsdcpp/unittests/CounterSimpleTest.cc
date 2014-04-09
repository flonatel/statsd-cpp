#include <gtest/gtest.h>
#include <statsdcpp/counter.hh>
#include <statsdcpp/collector.hh>
#include <statsdcpp/serializer/debug.hh>
#include <statsdcpp/output/string.hh>

class CounterSimple : public ::testing::Test {
};

TEST_F(CounterSimple, test_one_counter) {
   statsdcpp::output::string string_writer;
   statsdcpp::serializer::debug<statsdcpp::output::string>
      debug_serializer(string_writer);
   statsdcpp::collector< statsdcpp::serializer::debug<
      statsdcpp::output::string> >
      collector(debug_serializer);
   statsdcpp::counter_sp cnt(
      collector.generate_counter("org.flonatel.statsd.test.cnt1"));
   cnt->inc();

   collector.flush();

   ASSERT_EQ("Counter:org.flonatel.statsd.test.cnt1:1\n",
             string_writer.str());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
