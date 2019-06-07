import socket
import logging as log
import Paradigm_pb2
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
            raise Exception("Error connecting to pod configuration server : " + str(e))


    def send_configuration(self, configuration=DEFAULT_CONFIGURATION):
        if not self._connected:
            self._connect_to_pod()
        serialized_config = self.get_config_proto(configuration)
        try:
            self._pod_socket.sendall(serialized_config)
        except socket.error as e:
            log.info("Failed to send config : " + str(e))
            self.shutdown()
            self._connect_to_pod()


    @staticmethod
    def get_config_proto(config):
        flight_config = flightConfig()
        flight_config.retrievalTimeout = int(config['retrieval_timeout'])
        flight_config.maxFlightTime = int(config['max_flight_time'])
        flight_config.motorSpeed = int(config['motor_speed'])
        flight_config.pdsTelemetryPort = int(config['telemetry_port'])
        flight_config.commandPort = int(config['command_port'])
        flight_config.flightLength = int(config['flight_length'])
        flight_config.heartbeatTimeout = int(config['heartbeat_timeout'])
        flight_config.podDriver = config['pod_driver']
        return flight_config.SerializeToString()

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
