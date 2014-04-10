#ifndef STATSD_CPP_MEASURE_HH
#define STATSD_CPP_MEASURE_HH

#include <memory>
#include <statsdcpp/quantity.hh>
#include <statsdcpp/base.hh>

namespace statsdcpp {

// This is a generalization of the original statsd timer and gauge.
// It has the funtionality of timer but for arbritrary measurements.
template< typename TQuantity, typename TWriter >
class measure : public base< TWriter > {
public:
   measure(std::string const & name)
      : _name(name),
        _sum_val(0),
        _cnt_val(0) {
   }

   void add(TQuantity const & v) {
      if(_cnt_val==0) {
         _min = v;
         _max = v;
      }

      _min=std::min(_min, v);
      _max=std::max(_max, v);

      ++_cnt_val;
      _sum_val+=v;
   }

   uint64_t count_values() const { return _cnt_val; }
   TQuantity average() const {
      return _sum_val / statsdcpp::siunits::number(_cnt_val); }
   TQuantity max() const { return _max; }
   TQuantity min() const { return _min; }

   virtual void serialize_debug(
      TWriter & writer,
      std::chrono::system_clock::time_point const & begin_ts,
      std::chrono::system_clock::time_point const & end_ts) const;

private:
   std::string const _name;

   TQuantity _min;
   TQuantity _max;
   TQuantity _sum_val;
   uint64_t  _cnt_val;
};

template< typename TQuantity, typename TWriter >
using measure_sp = std::shared_ptr< measure< TQuantity, TWriter > >;


template< typename TQuantity, typename TWriter >
void measure< TQuantity, TWriter >::serialize_debug(
   TWriter & writer,
   std::chrono::system_clock::time_point const & /* begin_ts */,
   std::chrono::system_clock::time_point const & /* end_ts */) const {
   std::ostringstream ostr;
   ostr << "Measure:" << _name << ":" << _min.value() << ":" << _max.value()
        << ":" << average().value() << ":" << _cnt_val << std::endl;
   writer.write(ostr.str().c_str(), ostr.str().size());
}

}

#endif
