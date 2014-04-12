#include <gtest/gtest.h>
#include <statsdcpp/correlator.hh>
#include <statsdcpp/output/string.hh>
#include <statsdcpp/serializer/debug.hh>
#include <statsdcpp/collector.hh>

class CorrelatorTest : public ::testing::Test {
public:
   CorrelatorTest()
      : collector(string_writer) {
   }

   statsdcpp::output::string string_writer;
   statsdcpp::serializer::debug debug_serializer;
   statsdcpp::collector< statsdcpp::serializer::debug,
                         statsdcpp::output::string > collector;
};

TEST_F(CorrelatorTest, test_correlation) {
   using namespace statsdcpp::siunits;

   statsdcpp::correlator_sp<
      statsdcpp::output::string, int, std::string > crltr(
      collector.generate_correlator<int, std::string>(
         "org.flonatel.statsd.test.crltr"));

   crltr->in(7, "IN");
   crltr->in(9, "IN");
   crltr->in(12, "IN");
   crltr->in(15, "IN");

   usleep(100);

   crltr->seen(7, "in-queue");
   crltr->seen(9, "in-queue");
   crltr->seen(12, "in-queue");
   crltr->seen(17, "in-queue");

   usleep(70);

   crltr->seen(7, "odd-queue");
   crltr->seen(9, "odd-queue");
   crltr->seen(12, "even-queue");

   usleep(50);

   crltr->out(7, "OUT");
   crltr->out(9, "OUT");

   usleep(30);

   crltr->out(12, "OUT");

   collector.flush();

   // It's somewhat hard to implement a test because of the hole bunch
   // of times.

//   ASSERT_EQ("Measure:org.flonatel.statsd.test.msur1:0.02:0.04:0.03:3\n",
//             string_writer.str());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
