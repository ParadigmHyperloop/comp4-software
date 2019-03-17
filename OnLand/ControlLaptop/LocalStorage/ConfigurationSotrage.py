
"""
    Local File Storage Interface.
"""


class LocalStorage:
    @classmethod
    def get_default_configuration(cls):
        return DEFAULT_CONFIGURATION

    @classmethod
    def get_sensors(cls):
        return TempSensor()


# Working Sample Sensor Table Format.
class TempSensor:
    def __init__(self):
        self.name = 'name'
        self.min = 0
        self.max = 100


# Default Sample Configuration
DEFAULT_CONFIGURATION = {
    'air_resistance': 98,
    'motor_speed': 123,
    'tube_pressure': 456,
    'watchdog_timeout': 2000,
    'pod_address': '192.168.5.4'
}