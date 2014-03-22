// g++ -std=c++11 -O3 -Wall -Wextra -o udpserver udpserver.cc -lPocoNet

#include <thread>
#include <iostream>
#include <atomic>

#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>

std::atomic_ulong packets_received(0);

void count_packets() {
   while(true) {
      unsigned long const opr(packets_received.load());
      sleep(1);
      unsigned long const delta_packets_received
         = packets_received.load() - opr;
      std::cout << packets_received.load() << " "
                << delta_packets_received << std::endl;
   }
}

int main() {

   std::thread t(count_packets);

   Poco::Net::SocketAddress sa("127.0.0.1", 8125);
   Poco::Net::DatagramSocket dgs(sa);

   char buf[1024];

   while(true) {
      dgs.receiveBytes(buf, 1024);
      ++packets_received;
   }

   return 0;
}
