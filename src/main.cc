#include <pegtl.hh>

namespace statsd {

// Grammar for a counter:
// ([:alpha:]|[:digit:].)+:[:digit:]+|c

using namespace pegtl;

// This is simple, because currently only counters 'c' are supported.
struct read_type_char
   : string<'c'> {};

struct read_counter_increment
   : plus< digit > {};

struct read_counter_name
   : plus< sor< alnum, string<'.'> > > {};

struct read_statsd_line
   : seq<read_counter_name,
         string<':'>,
         read_counter_increment,
         string<'|'>,
         read_type_char,
         eol> {};

struct read_statsd_protocol
   : seq<read_statsd_line, eof> {};
}

int main() {
   pegtl::basic_parse_input< statsd::read_statsd_protocol >(
      std::istream_iterator< char >( std::cin ),
      std::istream_iterator< char >() );
   return 0;
}
