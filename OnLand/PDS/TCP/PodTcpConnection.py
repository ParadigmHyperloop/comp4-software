import socket
import select
import logging as log
from config import *
import errno


class PodTcpConnection:
    """
    PDS Pod Heartbeat and Command Sending
    A ping/pong tcp connection
    """

    def __init__(self, ip, port):
        self._sock = None
        self._pod_ip = ip
        self._pod_port = port
        self._connected = False

    def connect(self):
        try:
            self._sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except Exception as e:
            self.close()
            raise e
        try:
            self._sock.connect((self._pod_ip, self._pod_port))
        except ConnectionRefusedError:
            # No connection
            self.close()
            return False
        except Exception as e:
            self.close()
            return False

        self._sock.setblocking(False)
        self._connected = True
        return True

    def send_packet(self, payload):
        total_sent = 0
        if not self.is_connected():
            return
        while len(payload):
            try:
                sent = self._sock.send(payload)
                total_sent += sent
                payload = payload[sent:]
            except socket.error as e:
                if e.errno == errno.EAGAIN:
                    select.select([], [self._sock], [])
                    continue
                else:
                    self.close()
            except Exception as e:
                #todo log this or send to front end
                self.close()

    def is_connected(self):
        return self._connected

    def close(self):
        if self._sock is not None:
            self._connected = False
            self._sock.close()
            self._sock = None

    def receive(self):
        try:
            msg = self._sock.recv(MAX_MESSAGE_SIZE)
        except BlockingIOError:
            # When a non block socket doesnt receive anything it throws BlockingIOError
            return False
        except Exception as e:
            log.info(e)
            self.close()
            return
        else:  # No error, msg received
            if not msg:  # Empty message means that the connection was terminated by the pod
                log.info('Pod closed connection')
                self.close()
            else:
                return msg
