import logging as log
import socketio
import time
from config import *
from Paradigm_pb2 import *
from PDS.TCP.heartbeat_timer import HeartbeatTimer
from PDS.TCP.PodTcpConnection import PodTcpConnection
from PDS.config import COMMANDER_BACKUP_PULSE, COMMANDER_TIMEOUT_TIME, COMMANDER_PULSE_SPEED, POD_IP, POD_COMMANDER_PORT

#log.basicConfig(filename='logs\heartbeat.log', format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')
pod_command = podCommand()


# Create socket to connect to server
sio = socketio.Client()



@sio.on('connect')
def on_connect():
    log.warning("Front End: Connected")
    sio.emit('connected', "commander thread")


@sio.on('disconnect')
def on_disconnect():
    # Set the state to disconnected so that the pod knows?
    log.warning("Front End: Disconnected")


@sio.on('command')
def on_command(command):
    if command is '1':
        pod_command.manualBrakeNodeState = bnsVenting
        print("vent")
    else:
        pod_command.manualBrakeNodeState = bnsFlight
        print("Flight")


def main():
    log.warning("Heartbeat Thread Started")
    connected = False
    while not connected:
        try:
            sio.connect(SOCKET_SERVER)
        except:
            print("Commader cannot connect to SocketIO")
            time.sleep(2)
        else:
            connected = True

    pod_command.controlsInterfaceState = ciFlight

    pod = PodTcpConnection(ip=POD_IP, port=POD_COMMANDER_PORT)
    timer = HeartbeatTimer()

    while not pod.is_connected():
        time.sleep(1)
        if pod.connect():
            timer.pulse()

        while pod.is_connected():
            # Send a packets every PULSE_SPEED milliseconds.
            if timer.time_since_pulse() > COMMANDER_PULSE_SPEED:
                pod.send_packet(pod_command.SerializeToString())
                # Receive Packet
                while timer.time_since_pulse() > COMMANDER_PULSE_SPEED and pod.is_connected():
                    msg = pod.receive()
                    if not msg:
                        if timer.time_since_pulse() > COMMANDER_TIMEOUT_TIME:
                            print("Heartbeat: Timed out, after:" + str(timer.time_since_pulse()))
                            pod.close()
                        elif timer.time_since_pulse() > COMMANDER_BACKUP_PULSE:
                            print("Heartbeat: Sending backup packet")
                            pod.send_packet(pod_command.SerializeToString())
                    else:  # Msg received
                        sio.emit('ping', 1)
                        timer.pulse()
                        break
        # Connection lost, tell GUI
        sio.emit('ping', 0)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
