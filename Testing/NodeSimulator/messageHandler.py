from abc import ABCMeta, abstractmethod
from Paradigm_pb2 import *
from states import *


class MessageHandler(metaclass=ABCMeta):

    def __init__(self):
        self.values = dict()
        self.values['state'] = None

    @abstractmethod
    def parse_flight_computer_message(self, serialized_message):
        return self.values

    @abstractmethod
    def prepare_brake_node_message(self, brake_node_state):
        return self.values


class StringHandler(MessageHandler):

    def parse_flight_computer_message(self, serialized_message):
        message = serialized_message.decode('utf-8')

        if message == "Booting":
            return FlightComputerStates.BOOT
        elif message == "Standby":
            return FlightComputerStates.STANDBY
        elif message == "Arming":
            return FlightComputerStates.ARMING
        elif message == "Armed":
            return FlightComputerStates.ARMED
        elif message == "PreFlight":
            return FlightComputerStates.PREFLIGHT
        elif message == "Acceleration":
            return FlightComputerStates.ACCELERATION
        elif message == "Coasting":
            return FlightComputerStates.COASTING
        elif message == "Braking":
            return FlightComputerStates.BRAKING
        elif message == "Disarming":
            return FlightComputerStates.DISARMING
        elif message == "Retrieval":
            return FlightComputerStates.RETRIEVAL
        else:
            print("Failed to prase flight computer message: " + message)
            return FlightComputerStates.EMERGENCY

    def prepare_brake_node_message(self, brake_node_state):
        if brake_node_state == BrakeNodeStates.BOOT:
            return "0".encode()
        elif brake_node_state == BrakeNodeStates.STANDBY:
            return "1".encode()
        elif brake_node_state == BrakeNodeStates.ARMING:
            return "2".encode()
        elif brake_node_state == BrakeNodeStates.ARMED:
            return "3".encode()
        elif brake_node_state == BrakeNodeStates.FLIGHT:
            return "4".encode()
        elif brake_node_state == BrakeNodeStates.BRAKING:
            return "5".encode()
        elif brake_node_state == BrakeNodeStates.VENTING:
            return "6".encode()
        elif brake_node_state == BrakeNodeStates.RETRIEVAL:
            return "7".encode()
        else:
            return "8".encode()


class DtsNodeProtoHandler(MessageHandler):
    def parse_flight_computer_message(self, serialized_message):
        proto_message = fcToBrakeNode()
        proto_message.ParseFromString(serialized_message)
        return proto_message.manualNodeState

    def prepare_brake_node_message(self, node_update):
        proto_message = dtsNodeToFc()
        proto_message.brakeNodeState = node_update['State']
        proto_message.brakeSolenoidState = node_update['sol1']
        proto_message.ventSolenoidState = node_update['sol2']
        proto_message.rotorTemperature = node_update['rail_temp']
        proto_message.pressureTemperature = node_update['pressure_vessel_temp']
        proto_message.highPressure = node_update['hp']
        proto_message.lowPressure = node_update['lp']
        encoded = proto_message.SerializeToString()
        return encoded


