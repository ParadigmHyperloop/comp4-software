import socket
import NodeTelem_pb2
from readTelem import readProto
from writeTelem import createBreakTelem

UDP_IP = "127.0.0.1"
UDP_PORT = 5005

# Sample message load
breakNodeData = NodeTelem_pb2.brakeNodeData()
MESSAGE = createBreakTelem(breakNodeData).SerializeToString()

print("UDP target IP:", UDP_IP)
print("UDP target port:", UDP_PORT)
print("message:", MESSAGE)

# Setup socket & send
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # Internet, UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))