import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 8125
MESSAGE = "org.flonatel.statsd-cpp.test.performance.python.test01:1|c"
 
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

for i in xrange(0, 1000000):
    sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

