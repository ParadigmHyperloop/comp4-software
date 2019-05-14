import socket
import time
import random
from PDS.config import POD_COMMANDER_PORT
from PDS.UDP.Paradigm_pb2 import telemetry


def create_telem(packet):
    packet.lp1 = random.randint(1, 101)
    packet.hp = random.randint(1, 101)
    packet.sol1 = random.randint(0, 1)
    packet.pressureVesselTemperature = random.randint(1, 101)
    packet.railTemperature = random.randint(1, 101)
    packet.sol2 = random.randint(0, 1)

    return packet


def main():
    sudp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    stcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    stcp.bind(("127.0.0.1", POD_COMMANDER_PORT))
    stcp.listen()
    conn, addr = stcp.accept()
    print(conn, addr)

    pod_data = telemetry()

    while(1):
        create_telem(pod_data)
        sudp.sendto(pod_data.SerializeToString(), ("127.0.0.1", 4000))

        data = conn.recv(1024)

        if data:
            print("recved:", data)
            conn.send(b'1')

        time.sleep(1)

    stcp.close()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
