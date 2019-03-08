from abc import ABCMeta, abstractmethod


class Logger(metaclass=ABCMeta):

    def __init__(self, states_dict):
        self.last_reported = states_dict

    def log_states(self, states):
        update = False
        for key, state in states.items():
            if self.last_reported[key] is not states[key]:
                self.last_reported[key] = states[key]
                update = True
        if update:
            self.log()

    @abstractmethod
    def log(self):
        pass


class TerminalLogger(Logger):

    def log(self):
        for key, state in self.last_reported.items():
            print(key+": ", end="", flush=True)
            print(state, end="", flush=True)
            print("  |   ", end="", flush=True)
        print()
