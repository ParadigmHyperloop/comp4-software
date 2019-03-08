from networking import *

from hardwarestatesimulation import BrakeNode, FlightComputer
from behaviour import *

udp_connection = UdpConnection()
node_behaviour = NominalBrakeNodeBehaviour()
fc_behaviour = FlightComputerNominalBehaviour()

node = BrakeNode(node_behaviour)
fc = FlightComputer(fc_behaviour)

while True:
    connection_data = udp_connection.get_data()
    if connection_data:



    fc_state = fc.give_update()
    print(fc_state)

    node.parse_neighbor_update(fc_state)
    node_state = node.give_update()
    print(node_state)

    fc.parse_neighbor_update(node_state['State'])

