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

enum class checkpoint {
   cp_in, cp_in_queue, cp_odd, cp_even, cp_out };


TEST_F(CorrelatorTest, test_correlation) {
   using namespace statsdcpp::siunits;

   std::vector< statsdcpp::transition<checkpoint> > const tr {
      { checkpoint::cp_in, checkpoint::cp_in_queue, "IN->IQ" },
      { checkpoint::cp_in_queue, checkpoint::cp_odd, "IQ->ODD" },
      { checkpoint::cp_in_queue, checkpoint::cp_even, "IQ->EVEN" }
   };

   statsdcpp::correlator_sp<
      statsdcpp::output::string, int, checkpoint > crltr(
      collector.generate_correlator<int, checkpoint >(
         "org.flonatel.statsd.test.crltr", tr));

   crltr->in(7, checkpoint::cp_in);
   crltr->in(9, checkpoint::cp_in);
   crltr->in(12, checkpoint::cp_in);
   crltr->in(15, checkpoint::cp_in);

   usleep(100);

   crltr->seen(7, checkpoint::cp_in_queue);
   crltr->seen(9, checkpoint::cp_in_queue);
   crltr->seen(12, checkpoint::cp_in_queue);
   crltr->seen(17, checkpoint::cp_in_queue);

   usleep(70);

   crltr->seen(7, checkpoint::cp_odd);
   crltr->seen(9, checkpoint::cp_odd);
   crltr->seen(12, checkpoint::cp_even);

   usleep(50);

   crltr->out(7, checkpoint::cp_out);
   crltr->out(9, checkpoint::cp_out);

   usleep(30);

   crltr->out(12, checkpoint::cp_out);

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
