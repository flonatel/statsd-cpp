#ifndef STATSDCPP_BASE_HH
#define STATSDCPP_BASE_HH

#include <memory>
#include <chrono>

namespace statsdcpp {

// Common base class of all measureable objects.
// This is there because C++ currently lacks the possibility
// to put objects of different types into one container.
template< typename TWriter >
class base {
public:
   virtual ~base() {};

   virtual void serialize_debug(
      TWriter & writer,
      std::chrono::system_clock::time_point const & begin_ts,
      std::chrono::system_clock::time_point const & end_ts) const = 0;
};

template< typename TWriter>
using base_sp = std::shared_ptr< base< TWriter > >;

}

#endif
