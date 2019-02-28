from abc import ABCMeta, abstractmethod
from states import *
import time


class Node(metaclass=ABCMeta):

    def __init__(self):
        self.pod_state = None
        pass

    @abstractmethod
    def give_update(self):
        pass

    def parse_update(self, pod_state):
        self.pod_state = pod_state


class BrakeNode(Node):

    def __init__(self):
        self.sol1 = list()
        self.state = BrakeNodeStates.BOOT
        self.temperatures = list()

    def give_update(self):
        update = dict()
        update['Solenoids'] = self.sol1
        update['Temperatures'] = self.temperatures
        return update


class Transition(metaclass=ABCMeta):

    @abstractmethod
    def test_transition(self, current_flight_computer_state):


class ConditionalTransition(Transition):

    def __init__(self, transitioning_flight_computer_state, new_node_state):
        self.transitioning_flight_computer_state = transitioning_flight_computer_state
        self.new_node_state = new_node_state

    def test_transition(self, current_flight_computer_state):
        if current_flight_computer_state is self.transitioning_flight_computer_state:
            return self.new_node_state
        else:
            return False

class AutomaticTransition(Transition):

    def __init__(self, new_node_state):
        self.new_node_state = new_node_state

    def test_transition(self, current_flight_computer_state):
        return self.new_node_state

class TimedTransition(Transition):

    def __init__(self, transitioning_flight_computer_state, new_node_state, wait_time):
        self.transitioning_flight_computer_state = transitioning_flight_computer_state
        self.new_node_state = new_node_state
        self.wait_time = wait_time
        self.timer_on = False
        self.start_time = None

    def timer_complete(self):
        if self.timer_on and ((time.time() - self.start_time) > self.wait_time):
            return True
        else:
            return False

    def timer_running(self):
        return self.timer_on

    def start_timer(self):
        self.timer_on = True
        self.start_time = time.time()
        return

    def test_transition(self, current_flight_computer_state):
        if current_flight_computer_state is self.transitioning_flight_computer_state:
            if self.timer_complete():
                return self.new_node_state
            elif not self.timer_running():
                self.start_timer();
                return False
            else:
                return False
        else:
            if self.timer_running():
                self.reset_timer()
                return False
            return False


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





