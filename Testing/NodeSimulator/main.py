from networking import *

from hardwarestatesimulation import BrakeNode, NominalBrakeNodeBehaviour

udp_connection = UdpConnection()
nominal_behaviour = NominalBrakeNodeBehaviour()
node = BrakeNode(nominal_behaviour)

while True:
    udp_connection.send_data("hello".encode())


