import socket

from LocalStorage.ConfigurationSotrage import LocalStorage


class PodConstants:
    POD_COMMAND_PORT = 9999
    TEST_JSON_COMMAND_DICTIONARY = {'command': 2, "other_commands": 'test'}


class PodCommunicator:
    _pod_communicator_instance = None
    _pod_address = None
    _command_socket = None
    _queue = None

    def __init__(self,
                 pod_address=LocalStorage.get_default_configuration()['pod_address']):
        """ Virtual Private Constructor"""
        if PodCommunicator._pod_communicator_instance is not None:
            raise Exception("Pod-Communicator Instance Already exists")
        else:
            PodCommunicator._pod_communicator_instance = self
            self.pod_address = pod_address
            self._connect_to_pod()

    def _connect_to_pod(self):
        _command_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            _command_socket.connect(self._pod_address, PodConstants._POD_COMMAND_PORT)

            received = _command_socket.recv(1024)
        finally:
            pass

    def run(self):
        pass

    def _send_command(self, json_command):
        pass

    @staticmethod
    def get_pod_communicator():
        if PodCommunicator._pod_communicator_instance is None:
            PodCommunicator()
            return PodCommunicator._pod_communicator_instance

    def update_pod_address(self, new_address):
        self.pod_address = new_address

    def get_pod_address(self):
        return self._pod_address

