import socket
import NodeTelem_pb2
from google.protobuf.json_format import MessageToDict

# Setup socket
UDP_IP = "127.0.0.1"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Internet, UDP
sock.bind((UDP_IP, UDP_PORT))

# Listen for messages
while True:
    breakNodeData = NodeTelem_pb2.brakeNodeData()
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    breakNodeData.ParseFromString(data)
    
    # Sample print
    dataDict = MessageToDict(breakNodeData)
    for key, value in dataDict.items():
       print(key, "-", value)
