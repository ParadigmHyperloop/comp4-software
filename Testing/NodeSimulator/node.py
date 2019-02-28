from abc import ABCMeta, abstractmethod
from transitions import ConditionalTransition, TimedTransition, AutomaticTransition
from states import *


class Node(metaclass=ABCMeta):

    def __init__(self, behaviour):
        self.pod_state = None
        self.behaviour = behaviour
        pass

    @abstractmethod
    def give_update(self):
        pass

    def parse_update(self, pod_state):
        self.pod_state = pod_state


class BrakeNode(Node):

    def __init__(self, behaviour):
        super().__init__(behaviour)
        self.sol1 = list()
        self.state = BrakeNodeStates.BOOT
        self.temperatures = list()

    def give_update(self):
        update = dict()
        update['Solenoids'] = self.sol1
        update['Temperatures'] = self.temperatures
        return update


class BrakeNodeStateBehaviour:

    def __init__(self):
        self.transitions = dict()
        self.transitions[BrakeNodeStates.BOOT] = AutomaticTransition(BrakeNodeStates.STANDBY)
        self.transitions[BrakeNodeStates.STANDBY] = ConditionalTransition(FlightComputerStates.ARMING, BrakeNodeStates.ARMING)
        self.transitions[BrakeNodeStates.ARMING] = TimedTransition(FlightComputerStates.ARMING, BrakeNodeStates.ARMED, 10)
        self.transitions[BrakeNodeStates.ARMED] = ConditionalTransition(FlightComputerStates.ACCELERATION, BrakeNodeStates.FLIGHT)
        self.transitions[BrakeNodeStates.FLIGHT] = ConditionalTransition(FlightComputerStates.BRAKING, BrakeNodeStates.BRAKING)
        self.transitions[BrakeNodeStates.BRAKING] = ConditionalTransition(FlightComputerStates.DISARM, BrakeNodeStates.VENTING)
        self.transitions[BrakeNodeStates.VENTING] = TimedTransition(FlightComputerStates.DISARM, BrakeNodeStates.RETRIEVAL, 10)


    def sim(self, NodeState, FlightComputerState):
        available_transition = self.transitions[FlightComputerState]





