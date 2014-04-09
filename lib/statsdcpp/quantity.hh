#ifndef STATSD_QUANTITY_HH
#define STATSD_QUANTITY_HH

namespace statsdcpp {

// Minimalistic implementation of si - units
// i.e. everything what is measureable in this universe
// MKS system is used.
template<int M, int L, int T>
class quantity {
public:
   constexpr explicit quantity(double const v = 0.0)
      : _value(v) {}
   constexpr quantity(quantity const & other)
      : _value(other._value) {}

   quantity & operator+=(quantity const & other) {
      _value+=other._value;
      return *this;
   }

   constexpr double value() const { return _value; }

private:
   double _value;
};

template<int M1, int L1, int T1,
         int M2, int L2, int T2>
quantity<M1+M2, L1+L2, T1+T2> operator*(
   quantity<M1, L1, T1> const & l, quantity<M2, L2, T2> const & r) {
   return quantity<M1+M2, L1+L2, T1+T2>(l.value()*r.value());
}

template<int M, int L, int T>
quantity<M, L, T> operator+(quantity<M, L, T> const & l,
                            quantity<M, L, T> const & r) {
   return quantity<M, L, T>(l)+=r;
}

namespace siunits {

using number = quantity<0,0,0>;
using factor = quantity<0,0,0>;
using mass = quantity<1,0,0>;
using length = quantity<0,1,0>;
using area = quantity<0,2,0>;
using volume = quantity<0,3,0>;
using time = quantity<0,0,1>;
using speed = quantity<0,1,-1>;
using acceleration = quantity<0,1,-2>;
using frequency = quantity<0,0,-1>;
using force = quantity<1,1,-2>;
using pressure = quantity<1,-1,-2>;

constexpr factor percent(0.01);
constexpr length meter(1.0);

}

}

#endif
