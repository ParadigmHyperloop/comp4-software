import json
import socket

from LocalStorage.ConfigurationSotrage import DEFAULT_CONFIGURATION


class PodConnectionConstants:
    """ Pod Connection Constants"""
    POD_COMMAND_PORT = 65432
    POD_ADDRESS = '127.0.0.1'
    TEST_JSON_COMMAND_DICTIONARY = {'command': 2, "other_commands": 'test'}


class PodCommunicator:
    """ Pod Communicator - Handles Connection, sending Commands and Configs """
    _pod_communicator_instance = None
    _pod_address = None
    _pod_port = None
    _pod_socket = None
    _command_queue = None

    """DO NOT CALL"""
    def __init__(self, pod_address=PodConnectionConstants.POD_ADDRESS,
                 pod_port=PodConnectionConstants.POD_COMMAND_PORT):
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
            self._pod_socket.connect((self._pod_address, PodConnectionConstants.POD_COMMAND_PORT))
        except socket.error as e:
            print("Failed to Connect to Pod")
            raise e
        print('Connected...')

    def send_command(self, command):
        command_json = '{command: ' + command + '}'
        print(f'sending: {command_json}')
        self._pod_socket.sendall(command_json.encode())
        data = self._pod_socket.recv(1024)

    def send_configuration(self, configuration=DEFAULT_CONFIGURATION):
        try:
            self._pod_socket.sendall(json.dumps(configuration).encode())
        except socket.error as e:
            print("Failed to send config")
            raise e

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

