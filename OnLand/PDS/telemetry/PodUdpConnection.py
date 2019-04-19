from PDS.config import *
import logging
import threading
import socket
import select


class PodUdpConnection:
    """
    All PDS Pod Telemetry
    1-way UDP-Socket listening + some helper methods
    """
    def __init__(self, ip, port):
        self.sock = None
        self.ip = ip
        self.port = port
        self.lock = threading.Lock()

    def run(self, timeout=None):
        if not self.is_connected():
            return None
        if self.lock.acquire():
            try:
                data = self.recv(timeout=timeout)
            finally:
                self.lock.release()
            return data
        raise RuntimeError("Failed to acquire Lock on Channel %s" % self)

    def transcribe(data):
        logging.info("[DATA] {}".format(data))

    def recv(self, timeout=None):
        if not self.is_connected():
            return
        try:
            ready = select.select([self.sock], [], [], timeout.total_seconds())
            if ready[0]:
                data = self.sock.recvfrom(MAX_MESSAGE_SIZE)
                if data is not None:
                    return data
            else:
                return None

        except Exception as e:
            self.close()
            raise e

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
