import socket
import select


class PodTcpConnection:
    """
    PDS Pod Heartbeat and Command Sending
    A ping/pong tcp connection
    """

    def __init__(self, ip, port, MAX_MESSAGE_SIZE):
        self._sock = None
        self._pod_ip = ip
        self._pod_port = port
        self._connected = False
        self.max = MAX_MESSAGE_SIZE

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
            return False
        except Exception as e:
            self.close()
            raise e

        self._sock.setblocking(False)
        self._connected = True
        return True

    def send_packet(self, payload):
        total_sent = 0
        while len(payload):
            try:
                sent = self._sock.send(payload)
                total_sent += sent
                payload = payload[sent:]
            except socket.EAGAIN:
                # This blocks until the whole message is sent
                select.select([], [self._sock], [])
            except Exception as e:
                self.close()
                raise e

    def is_connected(self):
        return self._connected

    def close(self):
        if self._sock is not None:
            self._connected = False
            self._sock.close()
            self._sock = None

    def receive(self):
        try:
            msg = self._sock.recv()
        except BlockingIOError:
            # When a non block socket doesnt receive anything it throws BlockingIOError
            return False
        except Exception as e:
            self.close()
            raise e
        else:  # No error, msg received
            if not msg:  # Empty message means connection terminated by pod
                print('Pod closed connection')
                self.close()
            else:
                return msg
