
class PodStateType(type):
    MAP = {
        'psBooting': 0,
        'psStandby': 1,
        'psArming': 2,
        'psArmed': 3,
        'psFlight': 4,
        'psBraking': 5,
        'psVenting': 6,
        'psRetrieval': 7,
        'psError': 8,
    }

    def __getattr__(cls, name):
        if name in cls.MAP:
            return cls.MAP[name]
        raise AttributeError(name)


class PodState(metaclass=PodStateType):
    """
    Helper Methods for Pod states
    """
    def __init__(self, state):
        self.state = int(state)

    def is_fault(self):
        return self.state == PodState.EMERGENCY

    def is_moving(self):
        return self.state in (PodState.BRAKING, PodState.COASTING, PodState.PUSHING)

    def __str__(self):
        keys = [key for key, val in PodState.MAP.items() if val == self.state]
        if not keys:
            return "UNKNOWN"
        else:
            return keys[0]

    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return self.state == other.state
        return False