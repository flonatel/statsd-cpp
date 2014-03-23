#include <pegtl.hh>

namespace statsd {

// Grammar for a counter:
// ([:alpha:]|[:digit:]|.)+:[:digit:]+|c

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

struct set_counter_increment
   : action_base< set_counter_increment >  {
   static void apply(std::string const & m, statsd_data & s) {
      std::cout << "Set counter inc [" << m << "]" << std::endl;
      s.counter_increment = std::stol(m);
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
          one< ':' >,
          ifapply< read_counter_increment, set_counter_increment >,
          one< '|' >,
          read_type_char,
          eol> {};

struct read_statsd_protocol
   : seq<read_statsd_line, eof> {};
}

int main() {
   statsd::statsd_data data;

// My expextation is, that the both implementation do the same
// thing.
// call with:
// $ printf "h:93373|c\n" | ./bin/statsd

#if 0
// Looks that this is somewhat buggy:
// o Data in ifapply is always empty
// o Logs parse error at digit
   pegtl::basic_parse_input< statsd::read_statsd_protocol >(
      std::istream_iterator< char >( std::cin ),
      std::istream_iterator< char >(), data);
#else
   while(std::cin) {
      std::string line;
      std::getline(std::cin, line);
      if(!std::cin) break;
      pegtl::basic_parse_string< statsd::read_statsd_line >(
         line, data);
   }
#endif

   return 0;
}
