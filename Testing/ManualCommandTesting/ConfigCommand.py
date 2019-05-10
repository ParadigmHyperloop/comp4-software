import socket
import Paradigm_pb2

HOST = '127.0.0.1'
PORT = 3001

flight_config = Paradigm_pb2.flightConfig()
flight_config.retrieval_timeout = 1
flight_config.max_flight_time = 2
flight_config.motor_speed = 3
flight_config.telemetry_port = 4
flight_config.command_port = 5
flight_config.flight_length = 6
flight_config.heartbeat_timeout = 7
flight_config.pod_driver = Simulation

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(flight_config)
    data = s.recv(1024)

print('Received', repr(data))
input("Press Enter to continue...")
