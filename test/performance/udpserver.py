import socket
import time
import thread

UDP_IP = "127.0.0.1"
UDP_PORT = 8125

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

packets_received = 0

def count_packets():
    while True:
        opr = packets_received
        time.sleep(1.0)
        delta_packets_received = packets_received - opr
        print("%7d %7d" % (packets_received, delta_packets_received))

thread.start_new_thread(count_packets, ())

while True:
    data, addr = sock.recvfrom(1024)
    packets_received += 1
