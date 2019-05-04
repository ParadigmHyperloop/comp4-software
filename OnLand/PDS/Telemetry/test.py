import socket
import time
import random
from Paradigm_pb2 import *


states = [5,7,8]

def create_telem(packet):
    packet.podState = states[random.randint(0, 2)]
    packet.lp1 = random.randint(1, 101)
    packet.hp = random.randint(1, 101)
    packet.sol1 = random.randint(0, 1)
    packet.pressureVesselTemperature = random.randint(1, 101)
    packet.railTemperature = random.randint(1, 101)
    packet.sol2 = random.randint(0, 1)
    return packet


def main():
    UDP_IP = '127.0.0.1'
    UDP_PORT = 6000
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
    pod_data = telemetry()
    create_telem(pod_data)
    while(1):
        create_telem(pod_data)
        s.sendto(pod_data.SerializeToString(), ("127.0.0.1", 6000))
        time.sleep(2)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
