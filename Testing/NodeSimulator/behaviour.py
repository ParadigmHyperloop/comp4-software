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
        self.transitions[BrakeNodeStates.ARMING] = TimedTransition(FlightComputerStates.ARMING, BrakeNodeStates.ARMED, 4)
        self.transitions[BrakeNodeStates.ARMED] = ConditionalTransition(FlightComputerStates.ACCELERATION, BrakeNodeStates.FLIGHT)
        self.transitions[BrakeNodeStates.FLIGHT] = ConditionalTransition(FlightComputerStates.BRAKING, BrakeNodeStates.BRAKING)
        self.transitions[BrakeNodeStates.BRAKING] = ConditionalTransition(FlightComputerStates.DISARM, BrakeNodeStates.VENTING)
        self.transitions[BrakeNodeStates.VENTING] = TimedTransition(FlightComputerStates.DISARM, BrakeNodeStates.RETRIEVAL, 4)
        self.transitions[BrakeNodeStates.RETRIEVAL] = None


class FlightComputerNominalBehaviour(StateBehaviour):

    def __init__(self):
        super().__init__()
        self.transitions[FlightComputerStates.BOOT] = ConditionalTransition(BrakeNodeStates.STANDBY, FlightComputerStates.STANDBY)
        self.transitions[FlightComputerStates.STANDBY] = AutomaticTimeTransition(FlightComputerStates.ARMING, 3)
        self.transitions[FlightComputerStates.ARMING] = ConditionalTransition(BrakeNodeStates.ARMED, FlightComputerStates.ARMED)
        self.transitions[FlightComputerStates.ARMED] = AutomaticTimeTransition(FlightComputerStates.ACCELERATION, 3)
        self.transitions[FlightComputerStates.ACCELERATION] = AutomaticTimeTransition(FlightComputerStates.COASTING, 3)
        self.transitions[FlightComputerStates.COASTING] = AutomaticTimeTransition(FlightComputerStates.BRAKING, 1)
        self.transitions[FlightComputerStates.BRAKING] = AutomaticTimeTransition(FlightComputerStates.DISARM, 3)
        self.transitions[FlightComputerStates.DISARM] = ConditionalTransition(BrakeNodeStates.RETRIEVAL, FlightComputerStates.RETRIEVAL)
        self.transitions[FlightComputerStates.RETRIEVAL] = None

