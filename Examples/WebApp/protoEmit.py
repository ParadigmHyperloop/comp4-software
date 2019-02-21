import random
import socket

from Examples.WebApp import protoTest_pb2


def genNonProto():
    temp = []
    for x in range(0, 256):
        temp.append(random.randrange(0, 256))
    return temp


def genProtoPacket():
    # We start by creating a Protobuf Object, and set the properties,
    message = protoTest_pb2.telemetry()
    message.podState = 1
    message.sensor0Temp = random.randint(0, 256)
    message.sensor1Temp = random.randint(0, 256)
    message.sensor2Temp = random.randint(0, 256)
    message.sensor3Temp = random.randint(0, 256)
    message.sensor4Temp = random.randint(0, 256)
    message.sensor5Temp = random.randint(0, 256)
    message.sensor6Temp = random.randint(0, 256)
    message.sensor7Temp = random.randint(0, 256)
    message.sensor8Temp = random.randint(0, 256)
    message.sensor9Temp = random.randint(0, 256)
    message.sensor10Temp = random.randint(0, 256)
    return message


# print(message.ByteSize())
    # print(message.SerializeToString())
    # print(MessageToString(message))
    # return message

# Connect connect/listen to port and all that
UDP_IP = '127.0.0.1'
UDP_PORT = 6000
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
s.connect((UDP_IP, UDP_PORT))


# generate and send a Protobuf packet
## You have to SerializeToString before sending the packet !
temp = genProtoPacket()
# print(temp)
temp = temp.SerializeToString()
# print(temp)
s.send(temp)

# generate and send a byteArray packet, for comparison purposes
# temp = genNonProto()
# # print(temp)
# temp = bytearray(temp)
# # print(temp)
# # print(len(temp))
# s.send(temp)
