import socket
from Sources import NodeTelem_pb2

# Setup socket
UDP_IP = "127.0.0.1"
UDP_PORT = 6000

# SOCK_DGRAM indicates UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

# Listen for messages
while True:
    # this grabs the raw info from the socket
    data, addr = sock.recvfrom(1024)  # buffer size is 1024 bytes

    # and this parses the info according to the `.proto` structure
    breakNodeData = NodeTelem_pb2.brakeNodeData()
    breakNodeData.ParseFromString(data)

    # Sample print
    print(breakNodeData)
