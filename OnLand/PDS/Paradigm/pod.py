import socket
import logging
import select
import threading
from datetime import timedelta
from Sources import PodTelem_pb2
from google.protobuf.json_format import MessageToDict

MAX_MESSAGE_SIZE = 1024


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


class Pod:
    """
    All PDS Pod Telemetry
    Basically 1-way UDP-Socket listening + some helper methods
    """
    def __init__(self, ip, port):
        self.sock = None
        self.ip = ip
        self.port = port
        self.lock = threading.Lock()

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
            (_, ready, _) = select.select([], [self.sock], [], timeout.total_seconds())
            if self.sock in ready:
                data, addr = self.sock.recvfrom(MAX_MESSAGE_SIZE)

                if data is not None:
                    pod_data = PodTelem_pb2.telemetry()
                    pod_data.ParseFromString(data)
                    pod_data = MessageToDict(pod_data)
    
                    logging.debug("Recv {}".format(pod_data))
                    return pod_data

        except Exception as e:
            self.close()
            raise e

        self.close()
        return None

    def connect(self):
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

            self.sock.bind((self.ip, self.port))
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


def main():
    pod = Pod('127.0.0.1', 6000)
    pod.connect()
    
    while pod.is_connected():
        data = pod.recv(timedelta(seconds=0.01))
        if data is not None:
            print("Success!")
            for key, value in data.items():
                print(key, "-", value)

    pod.close()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
