from abc import ABCMeta, abstractmethod
import time

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
                self.start_timer()
                return False
            else:
                return False
        else:
            if self.timer_running():
                self.halt_timer()
                return False
            return False

    def halt_timer(self):
        self.timer_on = False
        return