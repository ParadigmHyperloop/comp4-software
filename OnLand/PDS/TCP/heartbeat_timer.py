import time


class HeartbeatTimer:

    def __init__(self):
        self._last_pulse = self._current_time_milli()

    @staticmethod
    def _current_time_milli():
        return int(round(time.time() * 1000))

    def time_since_pulse(self):
        time_elapsed = self._current_time_milli() - self._last_pulse
        return time_elapsed

    def pulse(self):
        self._last_pulse = self._current_time_milli()

