#ifndef STATSDCPP_BASE_HH
#define STATSDCPP_BASE_HH

#include <memory>
#include <chrono>
#include <string>

namespace statsdcpp {

// Common base class of all measureable objects.
// This is there because C++ currently lacks the possibility
// to put objects of different types into one container.
template< typename TWriter >
class base {
public:
   base(std::string const & name);
   virtual ~base() {};

   virtual void serialize_debug(TWriter & writer) = 0;
   virtual void serialize_carbon(TWriter & writer) = 0;

protected:
   std::string const _name;
   std::chrono::system_clock::time_point _begin_ts;
};

template< typename TWriter>
using base_sp = std::shared_ptr< base< TWriter > >;

// === Implementation

template< typename TWriter >
base< TWriter >::base(std::string const & name)
   : _name(name),
     _begin_ts(std::chrono::high_resolution_clock::now()) {
}

}

#endif
