from enum import Enum


class BrakeNodeStates(Enum):
    BOOT = 1
    STANDBY = 2
    ARMING = 3
    ARMED = 4
    FLIGHT = 5
    BRAKING = 6
    VENTING = 7
    RETRIEVAL = 8
    ERROR = 9


class FlightComputerStates(Enum):
    BOOT = 1
    STANDBY = 2
    ARMING = 3
    ARMED = 4
    PREFLIGHT = 5
    ACCELERATION = 6
    COASTING = 7
    BRAKING = 8
    DISARMING = 9
    RETRIEVAL = 10
    EMERGENCY = 11


