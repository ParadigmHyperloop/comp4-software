import socket
import logging as log
from ControlLaptop import Paradigm_pb2
from ControlLaptop.LocalStorage.ConfigurationSotrage import DEFAULT_CONFIGURATION
from config import POD_IP, POD_CONFIG_PORT

class PodCommunicator:
    """ Pod Communicator - Handles Sending Flight Configuration Configs """
    _pod_communicator_instance = None
    _pod_address = None
    _pod_port = None
    _pod_socket = None
    _connected = False

    """DO NOT CALL"""
    def __init__(self, pod_address=POD_IP,
                 pod_port=POD_CONFIG_PORT):
        if PodCommunicator._pod_communicator_instance is not None:
            raise Exception("Pod-Communicator Instance Already exists")
        else:
            PodCommunicator._pod_communicator_instance = self
            self._pod_address = pod_address
            self._pod_port = pod_port
            self._connect_to_pod()

    def _connect_to_pod(self):
        self._pod_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self._pod_socket.settimeout(2)
            self._pod_socket.connect((self._pod_address, self._pod_port))
            self._pod_socket.settimeout(None)
        except socket.error as e:
            log.warning("Failed to Connect to Pod")
            raise e
        log.info('Connected...'s)

    def send_command(self, command):
        command_json = '{command: ' + command + '}'
        log.info('sending: {command_json}')
        try:
            self._pod_socket.sendall(command_json.encode())
            data = self._pod_socket.recv(1024)
        except socket.error as e:
            log.warning("Failed to send command")
            self.shutdown()
            self._connect_to_pod()

    def send_configuration(self, configuration=DEFAULT_CONFIGURATION):
        try:
            self._pod_socket.sendall(serialized_config)
        except socket.error as e:
            log.warning("Failed to send config")
            self.shutdown()
            self._connect_to_pod()

    # Disconnect from Pod/Socket
    def shutdown(self):
        self._pod_socket.close()

    @staticmethod
    def get_pod_communicator():
        if PodCommunicator._pod_communicator_instance is None:
            PodCommunicator()
        return PodCommunicator._pod_communicator_instance

    def update_pod_address(self, new_address):
        self._pod_address = new_address

    def get_pod_address(self):
        return self._pod_address

