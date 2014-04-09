#include <gtest/gtest.h>
#include <statsdcpp/counter.hh>
#include <statsdcpp/collector.hh>
#include <statsdcpp/serializer/debug.hh>
#include <statsdcpp/output/string.hh>

class CounterSimple : public ::testing::Test {
public:
   CounterSimple()
      : debug_serializer(string_writer),
        collector(debug_serializer) {
   }

   statsdcpp::output::string string_writer;
   statsdcpp::serializer::debug<statsdcpp::output::string>
   debug_serializer;
   statsdcpp::collector< statsdcpp::serializer::debug<
                            statsdcpp::output::string> >
      collector;
};

TEST_F(CounterSimple, test_one_counter) {
   statsdcpp::counter_sp cnt(
      collector.generate_counter("org.flonatel.statsd.test.cnt1"));
   cnt->inc();

   collector.flush();

   ASSERT_EQ("Counter:org.flonatel.statsd.test.cnt1:1\n",
             string_writer.str());
}

TEST_F(CounterSimple, test_two_counter) {
   statsdcpp::counter_sp cnt1(
      collector.generate_counter("org.flonatel.statsd.test.cnt1"));
   cnt1->inc();
   statsdcpp::counter_sp cnt2(
      collector.generate_counter("org.flonatel.statsd.test.cnt2"));
   cnt2->inc();

   collector.flush();

   ASSERT_EQ("Counter:org.flonatel.statsd.test.cnt1:1\n"
             "Counter:org.flonatel.statsd.test.cnt2:1\n",
             string_writer.str());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
