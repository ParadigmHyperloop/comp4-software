from PodCommand_pb2 import podCommand
from States_pb2 import *
import select


import socket


TCP_IP = '127.0.0.1'
TCP_PORT = 5009
BUFFER_SIZE = 1024

# Generate packet
podMessage = podCommand()
podMessage.controlsInterfaceState = ciFlight

podSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
a = podSocket.connect((TCP_IP, TCP_PORT))
connected = True
try:
    podSocket.send(podMessage.SerializeToString())
except socket.error as e:
    #connected = False

while connected:
    try:
        pass
        # podSocket.send(podMessage.SerializeToString())
    except socket.error as e:
        connected = False
        break

    ready = select.select([podSocket], [], [], 3)
    if ready[0]:
        data = podSocket.recv(BUFFER_SIZE)
        print("received data:", data)

podSocket.close()




