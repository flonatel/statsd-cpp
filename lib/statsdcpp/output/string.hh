#ifndef STATSD_CPP_OUTPUT_STRING_HH
#define STATSD_CPP_OUTPUT_STRING_HH

namespace statsdcpp { namespace output {

class string {
public:
   std::string const & str() const { return _str; }

   void write(char const * mb, std::size_t const size) {
      _str.append(mb, size);
   }

private:
   std::string _str;
};

}}

#endif
