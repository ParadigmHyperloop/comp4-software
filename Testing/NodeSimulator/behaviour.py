from states import FlightComputerStates, BrakeNodeStates
from transitions import *


class StateBehaviour:

    def __init__(self):
        self.transitions = dict()

    def behave(self, current_state, neighbor_state):
        transition = self.transitions[current_state]
        if transition:
            new_state = transition.test_transition(neighbor_state)
            if new_state:
                return new_state
            else:
                return current_state
        else:
            return current_state


class NominalBrakeNodeBehaviour(StateBehaviour):

    def __init__(self):
        super().__init__()
        self.transitions[BrakeNodeStates.BOOT] = AutomaticTransition(BrakeNodeStates.STANDBY)
        self.transitions[BrakeNodeStates.STANDBY] = ConditionalTransition(FlightComputerStates.ARMING, BrakeNodeStates.ARMING)
        self.transitions[BrakeNodeStates.ARMING] = TimedTransition(FlightComputerStates.ARMING, BrakeNodeStates.ARMED, 10)
        self.transitions[BrakeNodeStates.ARMED] = ConditionalTransition(FlightComputerStates.ACCELERATION, BrakeNodeStates.FLIGHT)
        self.transitions[BrakeNodeStates.FLIGHT] = ConditionalTransition(FlightComputerStates.BRAKING, BrakeNodeStates.BRAKING)
        self.transitions[BrakeNodeStates.BRAKING] = ConditionalTransition(FlightComputerStates.DISARM, BrakeNodeStates.VENTING)
        self.transitions[BrakeNodeStates.VENTING] = TimedTransition(FlightComputerStates.DISARM, BrakeNodeStates.RETRIEVAL, 10)


class FlightComputerNominalBehaviour():

    def __init__(self):
        self.transitions = dict()
        self.transitions[FlightComputerStates.BOOT] = ConditionalTransition()
        self.transitions[FlightComputerStates.STANDBY] = ConditionalTransition(FlightComputerStates.ARMING, BrakeNodeStates.ARMING)
        self.transitions[FlightComputerStates.ARMING] = TimedTransition(FlightComputerStates.ARMING, BrakeNodeStates.ARMED, 10)
        self.transitions[FlightComputerStates.ARMED] = ConditionalTransition(FlightComputerStates.ACCELERATION, BrakeNodeStates.FLIGHT)
        self.transitions[FlightComputerStates.ACCELERATION] = ConditionalTransition(FlightComputerStates.BRAKING, BrakeNodeStates.BRAKING)
        self.transitions[FlightComputerStates.COASTING] = ConditionalTransition(FlightComputerStates.DISARM, BrakeNodeStates.VENTING)
        self.transitions[FlightComputerStates.BRAKING] = TimedTransition(FlightComputerStates.DISARM, BrakeNodeStates.RETRIEVAL, 10)
        self.transitions[FlightComputerStates.DISARM] = TimedTransition(FlightComputerStates.DISARM, BrakeNodeStates.RETRIEVAL, 10)
        self.transitions[FlightComputerStates.RETRIEVAL] = TimedTransition(FlightComputerStates.DISARM, BrakeNodeStates.RETRIEVAL, 10)
        self.transitions[FlightComputerStates.EMERGENCY] = TimedTransition(FlightComputerStates.DISARM, BrakeNodeStates.RETRIEVAL, 10)