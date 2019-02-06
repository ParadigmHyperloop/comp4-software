import socket
from Examples.WebApp import protoTest_pb2

# Creates the port and all that
UDP_IP = '127.0.0.1'
UDP_PORT = 6000
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

while True:
    message, add = sock.recvfrom(1024)
    # message(str(message))
    temp = protoTest_pb2.telemetry()
    temp.ParseFromString(message)
    print(temp)
    # print(message.ParseFromString())
