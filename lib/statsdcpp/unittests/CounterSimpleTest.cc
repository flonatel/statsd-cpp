#include <gtest/gtest.h>
#include <statsdcpp/counter.hh>
#include <statsdcpp/collector.hh>
#include <statsdcpp/output/string.hh>
#include <statsdcpp/serializer/debug.hh>

class CounterSimple : public ::testing::Test {
public:
   CounterSimple()
      : collector(string_writer) {
   }

   statsdcpp::output::string string_writer;
   statsdcpp::serializer::debug debug_serializer;
   statsdcpp::collector< statsdcpp::serializer::debug,
                         statsdcpp::output::string > collector;
};

TEST_F(CounterSimple, test_one_counter) {
   statsdcpp::counter_sp<statsdcpp::output::string> cnt(
      collector.generate_counter("org.flonatel.statsd.test.cnt1"));
   cnt->inc();

   collector.flush();

   ASSERT_EQ("Counter:org.flonatel.statsd.test.cnt1:1\n",
             string_writer.str());
}

TEST_F(CounterSimple, test_two_counter) {
   statsdcpp::counter_sp<statsdcpp::output::string> cnt1(
      collector.generate_counter("org.flonatel.statsd.test.cnt1"));
   cnt1->inc();
   statsdcpp::counter_sp<statsdcpp::output::string> cnt2(
      collector.generate_counter("org.flonatel.statsd.test.cnt2"));
   cnt2->inc();

   collector.flush();

   ASSERT_EQ("Counter:org.flonatel.statsd.test.cnt1:1\n"
             "Counter:org.flonatel.statsd.test.cnt2:1\n",
             string_writer.str());
}

TEST_F(CounterSimple, test_one_counter_inc77twice) {
   statsdcpp::counter_sp<statsdcpp::output::string> cnt(
      collector.generate_counter("org.flonatel.statsd.test.cnt1"));
   cnt->inc(77);
   cnt->inc(77);

   collector.flush();

   ASSERT_EQ("Counter:org.flonatel.statsd.test.cnt1:154\n",
             string_writer.str());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
