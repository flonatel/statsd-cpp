#include <pegtl.hh>

namespace statsd {

std::map< std::string, double > counters;

// Grammar for a counter:
// ([:alpha:]|[:digit:]|.)+:([:digit:]|.)+|c

using namespace pegtl;

struct statsd_data {
   std::string name;
   double      counter_increment;
   char        type;
};

struct set_name
   : action_base< set_name >  {
   static void apply(std::string const & m, statsd_data & s) {
      s.name = m;
   }
};

struct set_counter_increment
   : action_base< set_counter_increment >  {
   static void apply(std::string const & m, statsd_data & s) {
      s.counter_increment = std::stod(m);
   }
};

struct set_type_char
   : action_base< set_type_char >  {
   static void apply(std::string const & m, statsd_data & s) {
      s.type = m[0];
   }
};

struct increment_counter
   : action_base< increment_counter >  {
   static void apply(std::string const & /* m */, statsd_data & s) {
      auto const g(counters.find(s.name));
      if(g==counters.end()) {
         counters.insert(std::make_pair(s.name, s.counter_increment));
      } else {
         g->second += s.counter_increment;
      }
   }
};

// This is simple, because currently only counters 'c' are supported.
struct read_type_char
   : string<'c'> {};

struct read_counter_increment
// ToDo: this is not really the expression for double
   : plus< sor< digit, string<'.'> > > {};

struct read_name
   : plus< sor< alnum, string<'.'> > > {};

struct read_statsd_line
   : seq< ifapply< read_name, set_name >,
          one< ':' >,
          ifapply< read_counter_increment, set_counter_increment >,
          one< '|' >,
          ifapply< read_type_char, set_type_char >,
          apply< increment_counter >,
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

   // Dump data
   for( auto const counter : statsd::counters ) {
      std::cout << counter.first << ": " << counter.second << std::endl;
   }

   return 0;
}
