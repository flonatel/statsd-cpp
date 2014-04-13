#ifndef STATSD_CPP_MEASURE_HH
#define STATSD_CPP_MEASURE_HH

#include <memory>
#include <mutex>
#include <sstream>
#include <statsdcpp/quantity.hh>
#include <statsdcpp/base.hh>
#include <statsdcpp/timeutils.hh>

namespace statsdcpp {

// This is a generalization of the original statsd timer and gauge.
// It has the funtionality of timer but for arbritrary measurements.
template< typename TQuantity, typename TWriter >
class measure : public base< TWriter > {
public:
   measure(std::string const & name)
      : base< TWriter >(name),
        _sum_val(0),
        _cnt_val(0) {
   }

   void add(TQuantity const & v) {
      std::lock_guard< std::mutex > lock(_data_mutex);
      add_locked(v);
   }

   void add_locked(TQuantity const & v) {
      if(_cnt_val==0) {
         _min = v;
         _max = v;
      }

      _min=std::min(_min, v);
      _max=std::max(_max, v);

      ++_cnt_val;
      _sum_val+=v;
   }


   virtual void serialize_debug(TWriter & writer);
   virtual void serialize_carbon(TWriter & writer);

private:
   TQuantity average_locked() const {
      return _sum_val / statsdcpp::siunits::number(_cnt_val); }

   // A mutex is needed because the computation of the different
   // fields cannot be done atomic.
   std::mutex _data_mutex;

   TQuantity _min;
   TQuantity _max;
   TQuantity _sum_val;
   uint64_t  _cnt_val;
};

template< typename TQuantity, typename TWriter >
using measure_sp = std::shared_ptr< measure< TQuantity, TWriter > >;


template< typename TQuantity, typename TWriter >
void measure< TQuantity, TWriter >::serialize_debug(TWriter & writer) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   std::ostringstream ostr;
   ostr << "Measure:" << this->_name << ":" << _min.value()
        << ":" << _max.value()
        << ":" << average_locked().value() << ":" << _cnt_val << std::endl;
   writer.write(ostr.str().c_str(), ostr.str().size());
}

template< typename TQuantity, typename TWriter >
void measure< TQuantity, TWriter >::serialize_carbon(TWriter & writer) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   std::chrono::system_clock::time_point const end_ts(
      std::chrono::high_resolution_clock::now());
   std::chrono::system_clock::time_point const mid(
      timeutils::interval_middle(this->_begin_ts, end_ts));

   auto const now(std::chrono::duration_cast<std::chrono::seconds>(
                     mid.time_since_epoch()).count());

   std::ostringstream ostr;
   ostr << this->_name << ".min " << _min.value() << " " << now << std::endl
        << this->_name << ".max " << _max.value() << " " << now << std::endl
        << this->_name << ".average " << average_locked().value() << " " << now
        << std::endl;
   writer.write(ostr.str().c_str(), ostr.str().size());

   // And reset
   _sum_val = TQuantity(0);
   _cnt_val = 0;
   this->_begin_ts = end_ts;
}

}

#endif
