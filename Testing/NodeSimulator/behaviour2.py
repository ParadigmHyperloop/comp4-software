from Paradigm_pb2 import BrakeNodeStates, PodStates
from behaviour import StateBehaviour
from transitions import *


class NominalDtsNodeBehaviour(StateBehaviour):

    def behave(self, current_state, neighbor_state):
        if neighbor_state is not None:
            return neighbor_state
        else:
            return current_state

    def __init__(self):
        super().__init__()


