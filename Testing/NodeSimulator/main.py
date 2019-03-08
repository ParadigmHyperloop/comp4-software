from networking import *
from logger import TerminalLogger
from hardwarestatesimulation import BrakeNode, FlightComputer
from behaviour import *

udp_connection = UdpConnection()
node_behaviour = NominalBrakeNodeBehaviour()
fc_behaviour = FlightComputerNominalBehaviour()

node = BrakeNode(node_behaviour)
fc = FlightComputer(fc_behaviour)

logger = TerminalLogger({'FlightComputer': None, 'Node': None})

while True:
    fc_update = fc.give_update()
    fc_state = fc_update['State']
    node.parse_neighbor_update(fc_state)

    node_update = node.give_update()
    node_state = node_update['State']
    fc.parse_neighbor_update(node_state)

    states = {'FlightComputer': fc_state, 'Node': node_state}
    logger.log_states(states)

