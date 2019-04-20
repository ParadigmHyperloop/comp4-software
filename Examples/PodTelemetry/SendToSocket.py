import socket
from Sources import NodeTelem_pb2


def createBreakTelem(packet):
    packet.id = 1
    packet.state = 5

    packet.sol1 = 2
    packet.sol2 = 3
    packet.sol3 = 4
    packet.sol4 = 5
    packet.sol5 = 6
    packet.sol6 = 7

    packet.hp = 8
    packet.lp1 = 9
    packet.lp2 = 10
    packet.lp3 = 11
    packet.lp4 = 12

    packet.temp = 13

    return packet


def main():
    # Connect to port and all that
    UDP_IP = '127.0.0.1'
    UDP_PORT = 6000
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
    s.connect((UDP_IP, UDP_PORT))

    # Generate packet
    breakNodeData = NodeTelem_pb2.brakeNodeData()
    createBreakTelem(breakNodeData)

    # send through UDP
    s.send(breakNodeData.SerializeToString())

    # Write to file
    # Add telem object
    with open("nodeTelem.pb", "wb") as outfile:
        outfile.write(breakNodeData.SerializeToString())


main()
