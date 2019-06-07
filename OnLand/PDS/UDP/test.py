import socket
import time
import random
from Paradigm_pb2 import *
from config import UDP_TELEM_PORT, LOCAL_HOST


def create_telem(packet):
    packet.podState = psBraking
    packet.lowPressure1 = random.randint(1, 101)
    packet.highPressure = random.randint(1, 101)
    packet.solenoid1 = random.randint(0, 1)
    packet.pressureVesselTemperature = random.randint(1, 101)
    packet.solenoid2 = random.randint(0, 1)
    return packet


def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
    pod_data = Telemetry()
    create_telem(pod_data)
    while(1):
        create_telem(pod_data)
        s.sendto(pod_data.SerializeToString(), (LOCAL_HOST, UDP_TELEM_PORT))
        time.sleep(2)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
