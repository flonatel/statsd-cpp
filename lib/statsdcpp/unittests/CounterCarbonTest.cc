#include <gtest/gtest.h>
#include <vector>
#include <statsdcpp/counter.hh>
#include <statsdcpp/collector.hh>
#include <statsdcpp/output/string.hh>
#include <statsdcpp/serializer/carbon.hh>

inline std::vector<std::string> &
split(std::string const & s, char const delim,
      std::vector<std::string> & elems) {
   std::stringstream ss(s);
   std::string item;
   while( std::getline(ss, item, delim) ) {
      elems.push_back(item);
   }
   return elems;
}

inline std::vector<std::string>
split( std::string const & s, char const delim ) {
   std::vector<std::string> elems;
   split(s, delim, elems);
   return elems;
}

class CarbonSimple : public ::testing::Test {
public:
   CarbonSimple()
      : collector(string_writer) {
   }

   statsdcpp::output::string string_writer;
   statsdcpp::serializer::carbon debug_serializer;
   statsdcpp::collector< statsdcpp::serializer::carbon,
                         statsdcpp::output::string > collector;
};

TEST_F(CarbonSimple, test_one_counter_inc77twice) {
   statsdcpp::counter_sp cnt(
      collector.generate_counter("org.flonatel.statsd.test.cnt1"));
   cnt->inc(77);
   cnt->inc(77);

   // Because it is calculated for the measured interval,
   // sleep sometime not to get the things too big.
   usleep(100);

   collector.flush();

   std::vector<std::string> const lvec(split(string_writer.str(), ' '));
   ASSERT_EQ(3, lvec.size());
   ASSERT_EQ(lvec[0], "org.flonatel.statsd.test.cnt1");

   // Some rough heuristics.
   ASSERT_NEAR(550000, stod(lvec[1]), 450000);
   ASSERT_NEAR(time(0), stod(lvec[2]), 10);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
