#include <gtest/gtest.h>
#include <statsdcpp/quantity.hh>

class QuantitiySimple : public ::testing::Test {
public:
};

TEST_F(QuantitiySimple, test_basic_quantitiy) {

   using namespace statsdcpp::siunits;

   length const l1;
   length const l2(l1 + factor(2.0) * meter);

   ASSERT_EQ(l2.value(), 2.0);
}

TEST_F(QuantitiySimple, test_percent) {

   using namespace statsdcpp::siunits;

   number const n1(200.0);
   number const n2(n1 * (number(10.0) * percent));

   ASSERT_EQ(n2.value(), 20.0);
}

TEST_F(QuantitiySimple, test_percent_with_literal) {

   using namespace statsdcpp::siunits;

   number const n1(200.0);
   number const n2(n1 * 10.0_percent);

   ASSERT_EQ(n2.value(), 20.0);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
