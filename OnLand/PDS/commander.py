import json
import logging as log
import time

import socketio

from PDS.TCP.PodTcpConnection import PodTcpConnection
from PDS.helpers.heartbeat_timer import HeartbeatTimer
from Paradigm_pb2 import *
from config import COMMANDER_BACKUP_PULSE, COMMANDER_TIMEOUT_TIME, COMMANDER_PULSE_SPEED, POD_IP, POD_COMMANDER_PORT, \
    COMMANDER_BROADCAST_FREQUENCY, SOCKET_SERVER

log.basicConfig(stream=sys.stdout, format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')
pod_command = PodCommand()
pod = PodTcpConnection(ip=POD_IP, port=POD_COMMANDER_PORT)
connection_status = {'name': 'commander', 'status': 0}
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


@sio.on('manual_state_command')
def on_state_command(command):
    command = json.loads(command)
    new_pod_command = PodCommand()
    new_pod_command.hasCommand = True
    target = command['target']
    state = command['state']
    if target == 'brake_node':
        new_pod_command.manualBrakeNodeState = BrakeNodeStates.Value(state)
    elif target == 'pod':
        new_pod_command.manualPodState = PodStates.Value(state)
    elif target == 'lvdc_node':
        new_pod_command.manualLvdcNodeState = LvdcNodeStates.Value(state)
    elif target == 'interface_state':
        new_pod_command.controlsInterfaceState = ControlsInterfaceStates.Value(state)
    pod.send_packet(new_pod_command.SerializeToString())


@sio.on('manual_configuration_command')
def on_state_command(command):
    command = json.loads(command)
    new_pod_command = PodCommand()
    new_pod_command.hasCommand = True
    target = command['target']
    configuration = command['configuration']
    if target == 'brake_node':
        new_pod_command.solenoidConfiguration.extend(configuration)
    elif target == 'pod':
        new_pod_command.sensorOverrideConfiguration.extend(configuration)
    elif target == 'lvdc_node':
        new_pod_command.powerRailConfiguration.extend(configuration)
    pod.send_packet(new_pod_command.SerializeToString())


@sio.on('flight_profile_command')
def on_arm_command(profile):
    new_pod_command = PodCommand()
    new_pod_command.hasCommand = True
    new_pod_command.motorTorque = profile['motor_speed']
    new_pod_command.flightDistance = profile['flight_distance']
    new_pod_command.maxFlightTime = profile['max_flight_time']
    pod.send_packet(new_pod_command.SerializeToString())


def main():
    log.info("Heartbeat Thread Started")
    global pod_command
    global connection_status
    connected = False
    pod_heartbeat = HeartbeatTimer()

    while not connected:
        try:
            sio.connect(SOCKET_SERVER)
        except:
            log.info("Commander cannot connect to SocketIO")
            time.sleep(2)
        else:
            connected = True
            sio.emit('join_room', 'command_updates')

    while not pod.is_connected():
        time.sleep(1)
        if pod.connect():
            pod_heartbeat.pulse()
        log.warning("Connected")
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
                            connection_status['status'] = 1
                            sio.emit('connection_updates', json.dumps(connection_status))
                            broadcast_timer.pulse()
                        pod_heartbeat.pulse()
                        break

        # Connection lost, tell GUI
        connection_status['status'] = 0
        sio.emit('connection_updates', json.dumps(connection_status))
        log.info("Heartbeat : Connection Lost")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
