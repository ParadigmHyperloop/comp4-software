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
    ACCELERATION = 5
    COASTING = 6
    BRAKING = 7
    DISARM = 8
    RETRIEVAL = 9
    EMERGENCY = 10


