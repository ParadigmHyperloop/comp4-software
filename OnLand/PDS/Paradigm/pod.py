import socket
import logging
import select
import threading


MAX_MESSAGE_SIZE = 2048


class PodStateType(type):
    MAP = {
        'psBooting'  : 0,
        'psStandby'  : 1,
        'psArming'   : 2,
        'psArmed'    : 3,
        'psFlight'   : 4,
        'psBraking'  : 5,
        'psVenting'  : 6,
        'psRetrieval': 7,
        'psError'    : 8,
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
        return self.state in (PodState.BRAKING, PodState.COASTING,
                              PodState.PUSHING)

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


class Pod:
    """
    Pod Telemetry
    Basically 1-way UDP-Socket listening + some helper methods
    """
    def __init__(self, addr):
        self.sock = None
        self.addr = addr
        self.recieved = 0
        self.state = None
        self.lock = threading.Lock()
        self.timeout_handler = None

    def run(self, cmd, timeout=None):
        if not self.is_connected():
            return None

        if self.lock.acquire():
            try:
                self.send(cmd + "\n")
                data = self.recv(timeout=timeout)
            finally:
                self.lock.release()
            return data

        raise RuntimeError("Failed to acquire Lock on Channel %s" % self)

    def transcribe(self, data):
        logging.info("[DATA] {}".format(data))

    def recv(self, timeout=None):
        if not self.is_connected():
            return

        try:
            (ready, _, _) = select.select([self.sock], [], [],
                                          timeout.total_seconds())
            if self.sock in ready:
                data = self.sock.recv(MAX_MESSAGE_SIZE).decode('utf-8')
                logging.debug("Sending {}".format(data))
                return data

        except Exception as e:
            self.close()
            raise e

        self.close()
        return None

    def connect(self):
        try:
            self.sock = socket.create_connection(self.addr, 1)

            self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            if getattr(socket, 'SO_REUSEPORT', None) is not None:
                self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)

            self.recieved = 0
        except Exception as e:
            self.close()
            raise e

    def close(self):
        if self.sock is not None:
            self.state = None
            self.sock.close()
            self.sock = None

    def is_connected(self):
        return self.sock is not None and self.sock.fileno() >= 0

    def __str__(self):
        return "%s:%d" % self.addrport