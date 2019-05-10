from networking import *
from logger import TerminalLogger
from messageHandler import StringHandler, DtsNodeProtoHandler
from hardwareStateSimulation import DtsNode
from behaviour2 import *

udp_connection = UdpConnection(flight_computer_port=5001, node_sim_port=5000, flight_computer_ip='localhost')


dts_node_behaviour = NominalDtsNodeBehaviour()
node = DtsNode(dts_node_behaviour)

messageHandler = DtsNodeProtoHandler()

logger = TerminalLogger({'Manual Command state': None, 'NodeState': None})

manual_command_state = None
node_state = None
i = 0
while True:

    fc_update_message = udp_connection.get_data()
    if fc_update_message:
        manual_command_state = messageHandler.parse_flight_computer_message(fc_update_message)

    node.parse_neighbor_update(manual_command_state)
    node_update = node.give_update()
    node_state = node_update['State']

    states = {'Manual Command state': manual_command_state, 'NodeState': node_state}
    logger.log_states(states)

    node_message = messageHandler.prepare_brake_node_message(node_update)
    udp_connection.send_data(node_message)



