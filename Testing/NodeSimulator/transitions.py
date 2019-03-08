from abc import ABCMeta, abstractmethod
import time


class Transition(metaclass=ABCMeta):

    @abstractmethod
    def test_transition(self, current_flight_computer_state):
        pass


class ConditionalTransition(Transition):

    def __init__(self, triggering_neighbor_state, new_self_state):
        self.triggering_neighbor_state = triggering_neighbor_state
        self.new_state = new_self_state

    def test_transition(self, neighbor_state):
        if neighbor_state is self.triggering_neighbor_state:
            return self.new_state
        else:
            return False


class AutomaticTransition(Transition):

    def __init__(self, new_self_state):
        self.new_state = new_self_state

    def test_transition(self, neighbor_state):
        return self.new_state


class TimedTransition(Transition):

    def __init__(self, triggering_neighbor_state, new_state, wait_time):
        self.triggering_neighbor_state = triggering_neighbor_state
        self.new_state = new_state
        self.wait_time = wait_time
        self.timer_on = False
        self.start_time = None

    def timer_complete(self):
        if self.timer_on and ((time.time() - self.start_time) > self.wait_time):
            return True
        else:
            return False

    def reset_timer(self):
        self.start_time = None
        self.timer_on = False

    def timer_running(self):
        return self.timer_on

    def start_timer(self):
        self.timer_on = True
        self.start_time = time.time()
        return

    def test_transition(self, neighbor_state):
        if neighbor_state is self.triggering_neighbor_state:
            if self.timer_complete():
                self.reset_timer()
                return self.new_state
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


class AutomaticTimeTransition(TimedTransition):

    # noinspection PyMissingConstructor
    def __init__(self, new_state, wait_time):
        self.new_state = new_state
        self.wait_time = wait_time
        self.timer_on = False
        self.start_time = None

    def test_transition(self, neighbor_state=None):
        if self.timer_complete():
            return self.new_state
        elif not self.timer_running():
            self.start_timer()
            return False
