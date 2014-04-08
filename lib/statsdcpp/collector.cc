#include <statsdcpp/collector.hh>

namespace statsdcpp {

counter_sp collector::generate_counter(std::string const & name) {
   counter_sp cntr(std::make_shared<counter>(name));
   _counters.insert(std::pair<std::string, counter_sp>(name, cntr));
   return cntr;
}

}
