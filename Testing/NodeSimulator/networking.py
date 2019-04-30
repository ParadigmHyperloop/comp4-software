from abc import ABCMeta, abstractmethod
import socket


class Connection(metaclass=ABCMeta):

    @abstractmethod
    def get_data(self):
        pass

    @abstractmethod
    def send_data(self, data):
        pass


class UdpConnection(Connection):

    def __init__(self, flight_computer_port=5008, node_sim_port=5000, flight_computer_ip="localhost"):
        self.flight_computer_port = flight_computer_port
        self.flight_computer_ip = flight_computer_ip
        self.outbound_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.inbound_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.inbound_socket.setblocking(0)
        self.inbound_socket.bind(('0.0.0.0', node_sim_port))
        return

    def send_data(self, data):
        self.outbound_socket.sendto(data, (self.flight_computer_ip, self.flight_computer_port))

    def get_data(self):
        data = None
        try:
            data, addr = self.inbound_socket.recvfrom(1024)
        # No data available is read as an IOError. Since we want the socket to be NonBlocking, we ignore the error
        except IOError as e:
            pass
        return data

