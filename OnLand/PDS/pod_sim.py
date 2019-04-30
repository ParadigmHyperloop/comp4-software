import socket
import socketio
import time
import random
from PDS.UDP.Paradigm_pb2 import *


def create_telem(packet):
    packet.lp1 = random.randint(1, 101)
    packet.hp = random.randint(1, 101)
    packet.sol1 = random.randint(0, 1)
    packet.pressureVesselTemperature = random.randint(1, 101)
    packet.railTemperature = random.randint(1, 101)
    packet.sol2 = random.randint(0, 1)
    return packet


sio = socketio.Client()
@sio.on('connect')
def on_connect():
    print("Connected to Server")
    sio.emit('connected', "commander thread")


def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
    pod_data = telemetry()

    while(1):
        create_telem(pod_data)
        s.sendto(pod_data.SerializeToString(), ("127.0.0.1", 6000))
        s.sendto(b'1', ("127.0.0.1", 5005))
        time.sleep(2)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
