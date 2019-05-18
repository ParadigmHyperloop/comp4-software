from abc import ABCMeta, abstractmethod
from states import *
from Paradigm_pb2 import BrakeNodeStates as protoStates
import random

class HardwareStateSimulation(metaclass=ABCMeta):

    def __init__(self, behaviour):
        self.state = None
        self.neighbor_state = None
        self.behaviour = behaviour
        pass

    @abstractmethod
    def give_update(self):
        pass

    def parse_neighbor_update(self, neighbor_state):
        if neighbor_state is not None:
            self.neighbor_state = neighbor_state


class BrakeNode(HardwareStateSimulation):

    def __init__(self, behaviour):
        super().__init__(behaviour)
        self.sol1 = list()
        self.state = BrakeNodeStates.BOOT
        self.temperatures = list()

    def give_update(self):
        self.state = self.behaviour.behave(self.state, self.neighbor_state)
        update = dict()
        update['Solenoids'] = self.sol1
        update['Temperatures'] = self.temperatures
        update['State'] = self.state
        return update


class DtsNode(HardwareStateSimulation):

    def __init__(self, behaviour):
        super().__init__(behaviour)
        self.sol1 = False
        self.sol2 = False
        self.hp = 0
        self.lp = 0
        self.rail_temp = 0
        self.pressure_vessel_temp = 0
        a = protoStates.keys()
        self.state = protoStates.Value('bnsVenting')

    def give_update(self):
        self.state = self.behaviour.behave(self.state, self.neighbor_state)
        self.update_sensors()
        update = dict()
        update['State'] = self.state
        update['solenoid1'] = self.sol1
        update['highPressure'] = self.hp
        update['lowPressure1'] = self.lp
        update['pressureVesselTemperature'] = self.pressure_vessel_temp
        return update

    def update_sensors(self):
        self.sol1 = random.randint(0, 1)
        self.sol2 = random.randint(0, 1)
        self.hp = random.randint(1, 101)
        self.lp = random.randint(1, 101)
        self.rail_temp = random.randint(1, 101)
        self.pressure_vessel_temp = random.randint(1, 101)



class FlightComputer(HardwareStateSimulation):

    def __init__(self, behaviour):
        super().__init__(behaviour)
        self.state = FlightComputerStates.BOOT

    def give_update(self):
        self.state = self.behaviour.behave(self.state, self.neighbor_state)
        update = dict()
        update['State'] = self.state
        return update

