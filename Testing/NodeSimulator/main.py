from Networking import *

udp_connection = UdpConnection()
while True:
    udp_connection.send_data("hello".encode())

