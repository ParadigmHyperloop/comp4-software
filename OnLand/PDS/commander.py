import logging as log
import socketio
import time
from PDS.config import SOCKET_SERVER
from PDS.Paradigm_pb2 import podCommand, ciFlight, bnsBraking, bnsStandby
from PDS.TCP.heartbeat_timer import HeartbeatTimer
from PDS.TCP.PodTcpConnection import PodTcpConnection
from PDS.config import COMMANDER_BACKUP_PULSE, COMMANDER_TIMEOUT_TIME, COMMANDER_PULSE_SPEED, POD_IP, POD_COMMANDER_PORT

log.basicConfig(filename='logs\paradigm.log', format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')
pod_message = podCommand()


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
    if command == '1':
        pod_message.manualBrakeNodeState = bnsBraking
    else:
        pod_message.manualBrakeNodeState = bnsStandby

def main():
    log.warning("Heartbeat Thread Started")
    connected = False
    while not connected:
        try:
            sio.connect(SOCKET_SERVER)
        except:
            log.warning("Commader cannot connect to SocketIO")
            time.sleep(2)
        else:
            connected = True


    pod_message = podCommand()
    pod_message.controlsInterfaceState = ciFlight

    pod = PodTcpConnection(ip=POD_IP, port=POD_COMMANDER_PORT)
    timer = HeartbeatTimer()

    while not pod.is_connected():
        time.sleep(1)
        if pod.connect():
            timer.pulse()

        while pod.is_connected():
            # Send a packets every PULSE_SPEED milliseconds.
            log.debug("Heartbeat: healthy")
            if timer.time_since_pulse() > COMMANDER_PULSE_SPEED:
                pod.send_packet(pod_message.SerializeToString())

                # Receive Packet
                while timer.time_since_pulse() > COMMANDER_PULSE_SPEED and pod.is_connected():
                    msg = pod.receive()
                    log.debug("Heartbeat: Received - " + str(msg))
                    if not msg:
                        if timer.time_since_pulse() > COMMANDER_TIMEOUT_TIME:
                            log.warning("Heartbeat: Timed out, after:" + str(timer.time_since_pulse()))
                            pod.close()
                        elif timer.time_since_pulse() > COMMANDER_BACKUP_PULSE:
                            log.debug("Heartbeat: Sending backup packet")
                            pod.send_packet(pod_message.SerializeToString())
                    else:  # Msg received
                        sio.emit('ping', 1)
                        timer.pulse()
                        break
        # Connection lost, tell GUI
        log.warning("Heartbeat: Lost")
        sio.emit('ping', 0)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
