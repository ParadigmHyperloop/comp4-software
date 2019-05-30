import logging as log
import time

import socketio

from PDS.TCP.PodTcpConnection import PodTcpConnection, COMMANDER_BROADCAST_FREQUENCY, SOCKET_SERVER
from PDS.helpers.heartbeat_timer import HeartbeatTimer
from Paradigm_pb2 import *
from config import COMMANDER_BACKUP_PULSE, COMMANDER_TIMEOUT_TIME, COMMANDER_PULSE_SPEED, POD_IP, POD_COMMANDER_PORT

log.basicConfig(stream=sys.stdout, format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p', level=log.INFO)
pod_command = PodCommand()
pod = PodTcpConnection(ip=POD_IP, port=POD_COMMANDER_PORT)
sio = socketio.Client()
broadcast_timer = HeartbeatTimer()
interface_state = ciStandby


@sio.on('connect')
def on_connect():
    log.info("Commander Thread: Connected to SocketIO")
    sio.emit('connected', "commander thread")


@sio.on('disconnect')
def on_disconnect():
    # Set the state to disconnected so that the pod knows?
    log.info("Commander Thread: Disconnected from SocketIO")
    sio.emit('ping', 0)


@sio.on('command')
def on_command(command):
    if command is '1':
        pod_command.manualBrakeNodeState = bnsVenting
        log.info("vent")
    else:
        pod_command.manualBrakeNodeState = bnsFlight
        log.info("Flight")
    pod.send_packet(pod_command.SerializeToString())


def main():
    global pod_command
    log.info("Heartbeat Thread Started")

    connected = False
    while not connected:
        try:
            sio.connect(SOCKET_SERVER)
        except ConnectionError as e:
            log.info("Commander cannot connect to SocketIO")
            time.sleep(2)
        else:
            connected = True

    pod_heartbeat = HeartbeatTimer()
    while not pod.is_connected():
        time.sleep(1)
        if pod.connect():
            pod_heartbeat.pulse()

        while pod.is_connected():
            pod_command.controlsInterfaceState = interface_state
            if pod_heartbeat.time_since_pulse() > COMMANDER_PULSE_SPEED:
                pod.send_packet(pod_command.SerializeToString())
                while pod_heartbeat.time_since_pulse() > COMMANDER_PULSE_SPEED and pod.is_connected():
                    msg = pod.receive()
                    log.debug("Heartbeat: Received - " + str(msg))
                    if not msg:
                        if pod_heartbeat.time_since_pulse() > COMMANDER_TIMEOUT_TIME:
                            log.info("Heartbeat: Timed out, after:" + str(pod_heartbeat.time_since_pulse()))
                            pod.close()
                        elif pod_heartbeat.time_since_pulse() > COMMANDER_BACKUP_PULSE:
                            log.warning("Heartbeat: Sending backup packet")
                            pod.send_packet(pod_command.SerializeToString())
                    else:  # Msg received
                        if broadcast_timer.time_since_pulse() > COMMANDER_BROADCAST_FREQUENCY:
                            sio.emit('ping', 1)
                            broadcast_timer.pulse()
                        pod_heartbeat.pulse()
                        break
        # Connection lost, tell GUI
        sio.emit('ping', 0)
        log.info("Heartbeat : Connection Lost")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
