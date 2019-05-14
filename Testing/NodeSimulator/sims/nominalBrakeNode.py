from networking import *
from logger import TerminalLogger
from messageHandler import StringHandler
from hardwareStateSimulation import BrakeNode, FlightComputer
from behaviour import *

udp_connection = UdpConnection(flight_computer_port=5001, node_sim_port=5000, flight_computer_ip='localhost')
node_behaviour = NominalBrakeNodeBehaviour()
node = BrakeNode(node_behaviour)

# fc_behaviour = FlightComputerNominalBehaviour()
# fc = FlightComputer(fc_behaviour)

messageHandler = StringHandler()

logger = TerminalLogger({'FlightComputer': None, 'Node': None})

fc_state = None
node_state = None
i = 0
while True:

    fc_update_message = udp_connection.get_data()
    if fc_update_message:
        print(fc_update_message)
        fc_state = messageHandler.parse_flight_computer_message(fc_update_message)

    node.parse_neighbor_update(fc_state)
    node_update = node.give_update()
    node_state = node_update['State']

    states = {'FlightComputer': fc_state, 'Node': node_state}
    logger.log_states(states)

    node_message = messageHandler.prepare_brake_node_message(node_state)
    udp_connection.send_data(str(i).encode())
    i = i + 1
    print(i)


