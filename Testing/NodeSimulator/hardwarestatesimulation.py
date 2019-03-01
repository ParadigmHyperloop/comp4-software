from abc import ABCMeta, abstractmethod
from states import *


class HardwareStateSimulation(metaclass=ABCMeta):

    def __init__(self, behaviour):
        self.neighbor_state = None
        self.behaviour = behaviour
        pass

    @abstractmethod
    def give_update(self):
        pass

    def parse_neighbor_update(self, pod_state):
        self.neighbor_state = pod_state


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


class FlightComputer(HardwareStateSimulation):

    def give_update(self):
        return self.behaviour.behave(self.state, self.neighbor_state)

