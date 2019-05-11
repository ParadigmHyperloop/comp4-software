import socketio
import time
from PDS.UDP.Paradigm_pb2 import *
from PDS.TCP.heartbeat_timer import HeartbeatTimer
from PDS.TCP.PodTcpConnection import PodTcpConnection
from PDS.config import *

# Create socket to connect to server
sio = socketio.Client()
@sio.on('connect')
def on_connect():
    print("Connected to Server")
    sio.emit('connected', "commander thread")


@sio.on('disconnect')
def on_disconnect():
    # Set the state to disconnected so that the pod knows?
    print('I\'m disconnected!')


@sio.on('command')
def on_command(command):
    if command is '1':
        podMessage.manualBrakeNodeState = bnsBraking
    else:
        podMessage.manualBrakeNodeState = bnsStandby


sio.connect('http://localhost:5000')
podMessage = podCommand()
podMessage.controlsInterfaceState = ciFlight


def main():
    print(POD_IP, POD_COMMANDER_PORT)
    pod = PodTcpConnection(ip=POD_IP, port=POD_COMMANDER_PORT)
    timer = HeartbeatTimer()

    while not pod.is_connected():
        time.sleep(1)
        print("attempting to reconnect...")
        pod.connect()

        while pod.is_connected():
            print('connected!')

            # Send a packets every PULSE_SPEED milliseconds.
            if timer.time_since_pulse() > COMMANDER_PULSE_SPEED:
                pod.send_packet(podMessage.SerializeToString())

                # Receive Packet
                while timer.time_since_pulse() > COMMANDER_PULSE_SPEED and pod.is_connected():
                    msg = pod.receive()
                    print("message:", msg)
                    if not msg:
                        if timer.time_since_pulse() > COMMANDER_TIMEOUT_TIME:
                            pod.close()
                        elif timer.time_since_pulse() > COMMANDER_BACKUP_PULSE:
                            pod.send_packet(podMessage.SerializeToString())
                            print("Sent packet!")
                    else:  # Msg received
                        sio.emit('ping', 1)
                        timer.pulse()
        # Connection lost, tell GUI
        sio.emit('ping', 0)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
