from PodCommand_pb2 import podCommand
from States_pb2 import *

import socket


TCP_IP = '127.0.0.1'
TCP_PORT = 5009
BUFFER_SIZE = 1024

# Generate packet
podMessage = podCommand()
podMessage.controlsInterfaceState = ciFlight

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
s.send(podMessage.SerializeToString())
data = s.recv(BUFFER_SIZE)
s.close()

print("received data:", data)



