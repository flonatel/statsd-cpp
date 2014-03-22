// g++ -O3 -Wall -Wextra -o udpclient udpclient.cc -lPocoNet

#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>

char const msg[] = "org.flonatel.statsd-cpp.test.performance.python.test01:1|c";
std::size_t const mlen = sizeof(msg);

int main() {

   Poco::Net::SocketAddress sa("127.0.0.1", 8125);
   Poco::Net::DatagramSocket dgs;

   for(unsigned long i(0); i<1000000; ++i) {
      dgs.sendTo(msg, mlen, sa);
   }

   return 0;
}
