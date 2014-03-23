#include <pegtl.hh>

namespace statsd {

// Grammar for a counter:
// ([:alpha:]|[:digit:].)+:[:digit:]+|c

using namespace pegtl;

struct statsd_data {
   std::string name;
   uint64_t    counter_increment;
   char        type;
};

struct set_name
   : action_base< set_name >  {
   static void apply(std::string const & m, statsd_data & s) {
      s.name = m;
      std::cout << "Set name [" << m << "]" << std::endl;
   }
};

// This is simple, because currently only counters 'c' are supported.
struct read_type_char
   : string<'c'> {};

struct read_counter_increment
   : plus< digit > {};

struct read_name
   : plus< sor< alnum, string<'.'> > > {};

struct read_statsd_line
   : seq< ifapply< read_name, set_name >,
          string<':'>,
          read_counter_increment,
          string<'|'>,
          read_type_char,
          eol> {};

struct read_statsd_protocol
   : seq<read_statsd_line, eof> {};
}

int main() {
   statsd::statsd_data data;
   pegtl::trace_parse_input< statsd::read_statsd_protocol >(true,
      std::istream_iterator< char >( std::cin ),
      std::istream_iterator< char >(), data);
   return 0;
}
