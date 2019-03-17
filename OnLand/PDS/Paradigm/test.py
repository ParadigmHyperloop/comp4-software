import socket
from Sources import PodTelem_pb2


def create_telem(packet):
    packet.podState = 69
    packet.terminalState = 69
    packet.podTime = 69
    packet.flightTime = 69
    packet.inverterON = 69
    packet.watchdogTimerValue = 69
    packet.watchdogFlag = 69
    packet.motorState = 69
    packet.inverterState = 69

    return packet


def main():
    UDP_IP = '127.0.0.1'
    UDP_PORT = 6000
    
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
    s.connect((UDP_IP, UDP_PORT))
    
    pod_data = PodTelem_pb2.telemetry()
    create_telem(pod_data)
    
    s.send(pod_data.SerializeToString())


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
