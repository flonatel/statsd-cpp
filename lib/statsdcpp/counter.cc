#include <statsdcpp/counter.hh>

namespace statsdcpp {

counter::counter(std::string const & name)
   : _name(name),
     _cnt(0) {
}

}
