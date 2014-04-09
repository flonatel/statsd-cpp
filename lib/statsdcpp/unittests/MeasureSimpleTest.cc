#include <gtest/gtest.h>
#include <statsdcpp/measure.hh>

class MeasureSimple : public ::testing::Test {
public:
};

TEST_F(MeasureSimple, test_basic_quantitiy) {

   using namespace statsdcpp::siunits;

   length const l1;
   length const l2(l1 + factor(2.0) * meter);

   ASSERT_EQ(l2.value(), 2.0);
}

TEST_F(MeasureSimple, test_percent) {

   using namespace statsdcpp::siunits;

   number const n1(200.0);
   number const n2(n1 * (number(10.0) * percent));

   ASSERT_EQ(n2.value(), 20.0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
