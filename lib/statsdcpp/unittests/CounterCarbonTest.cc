#include <gtest/gtest.h>
#include <vector>
#include <statsdcpp/counter.hh>
#include <statsdcpp/measure.hh>
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
   statsdcpp::counter_sp<statsdcpp::output::string> cnt(
      collector.generate_counter("org.flonatel.statsd.test.cnt1"));
   cnt->inc(77);
   cnt->inc(77);

   // Because it is calculated for the measured interval,
   // sleep sometime not to get the things too big.
   usleep(100);

   collector.flush();

   std::vector<std::string> const lines(split(string_writer.str(), '\n'));
   ASSERT_EQ(3, lines.size());

   {
      // Check first line
      std::vector<std::string> const lvec(split(lines[0], ' '));
      ASSERT_EQ(3, lvec.size());
      ASSERT_EQ(lvec[0], "org.flonatel.statsd.test.cnt1.persec");
      // Some rough heuristics.
      ASSERT_NEAR(550000, stod(lvec[1]), 450000);
      ASSERT_NEAR(time(0), stod(lvec[2]), 10);
   }

   {
      // Check second line
      std::vector<std::string> const lvec(split(lines[1], ' '));
      ASSERT_EQ(3, lvec.size());
      ASSERT_EQ(lvec[0], "org.flonatel.statsd.test.cnt1.absolut");
      ASSERT_EQ(154, stod(lvec[1]));
      ASSERT_NEAR(time(0), stod(lvec[2]), 10);
   }

   {
      // Check third line
      std::vector<std::string> const lvec(split(lines[2], ' '));
      ASSERT_EQ(3, lvec.size());
      ASSERT_EQ(lvec[0], "org.flonatel.statsd.test.cnt1.interval");
      ASSERT_NEAR(180000, stod(lvec[1]), 100000);
      ASSERT_NEAR(time(0), stod(lvec[2]), 10);
   }

}

TEST_F(CarbonSimple, test_one_measure) {
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

   std::vector<std::string> const lines(split(string_writer.str(), '\n'));
   ASSERT_EQ(3, lines.size());

   {
      // Check first line
      std::vector<std::string> const lvec(split(lines[0], ' '));
      ASSERT_EQ(3, lvec.size());
      ASSERT_EQ(lvec[0], "org.flonatel.statsd.test.msur1.min");
      ASSERT_NEAR(0.02, stod(lvec[1]), 0.0001);
      ASSERT_NEAR(::time(0), stod(lvec[2]), 10);
   }

   {
      // Check second line
      std::vector<std::string> const lvec(split(lines[1], ' '));
      ASSERT_EQ(3, lvec.size());
      ASSERT_EQ(lvec[0], "org.flonatel.statsd.test.msur1.max");
      ASSERT_NEAR(0.04, stod(lvec[1]), 0.0001);
      ASSERT_NEAR(::time(0), stod(lvec[2]), 10);
   }

   {
      // Check third line
      std::vector<std::string> const lvec(split(lines[2], ' '));
      ASSERT_EQ(3, lvec.size());
      ASSERT_EQ(lvec[0], "org.flonatel.statsd.test.msur1.average");
      ASSERT_NEAR(0.03, stod(lvec[1]), 0.0001);
      ASSERT_NEAR(::time(0), stod(lvec[2]), 10);
   }
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
