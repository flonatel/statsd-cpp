#ifndef STATSD_CPP_OUTPUT_FD_HH
#define STATSD_CPP_OUTPUT_FD_HH

#include <unistd.h>

namespace statsdcpp { namespace output {

class fd {
public:
   fd(int f)
   : _fd(f) {}

   void write(char const * mb, std::size_t const size) {
      if(_fd==-1) {
         return;
      }

      ssize_t const wr = ::write(_fd, mb, size);

      if(wr==-1) {
         perror("write - closing fd");
         close(_fd);
         _fd=-1;
         return;
      }

      if(static_cast<std::size_t>(wr)!=size) {
         perror("Could not write all data");
         close(_fd);
         _fd=-1;
         return;
      }
   }

private:
   int _fd;
};

}}

#endif
