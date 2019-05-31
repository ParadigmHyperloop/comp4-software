import socketio
import time
import logging as log
from datetime import timedelta
from config import SOCKET_SERVER, POD_IP, UDP_TELEM_PORT, MAX_MESSAGE_SIZE, UDP_TELEM_TIMEOUT, TELEMETRY_BROADCAST_FREQUENCY
from PDS.UDP.PodUdpConnection import PodUdpConnection
from Paradigm_pb2 import Telemetry
from helpers.heartbeat_timer import HeartbeatTimer
from google.protobuf import json_format
from google.protobuf.json_format import MessageToDict
import sys
import json

broadcast_timer = HeartbeatTimer()
log.basicConfig(stream=sys.stdout, format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')

# Create socket to connect to server
sio = socketio.Client()
connection_status = {'name': 'telemetry', 'status': 0}

@sio.on('connect')
def on_connect():
    log.warning("Front End: Connected")
    sio.emit('connected', "telemetry")


@sio.on('disconnect')
def on_disconnect():
    sio.emit('telemetry_connection', '0')
    log.warning("Front End: Disconnected")


def main():
    log.warning("Telemetry Thread Started")
    connected = False
    while not connected:
        try:
            sio.connect(SOCKET_SERVER) # Default namespace only
        except:
            log.warning("Telemetry thread cannot connect to SocketIO")
            time.sleep(2)
        else:
            connected = True

    udp_socket = PodUdpConnection(POD_IP, UDP_TELEM_PORT, MAX_MESSAGE_SIZE)
    udp_socket.connect()
    if udp_socket.is_connected():
        connection_status['status'] = 1
        sio.emit('connection_updates', json.dumps(connection_status))
    while udp_socket.is_connected():
        data = udp_socket.recv(timedelta(seconds=UDP_TELEM_TIMEOUT))
        if data is not None:
            if broadcast_timer.time_since_pulse() > TELEMETRY_BROADCAST_FREQUENCY:
                broadcast_timer.pulse()
                pod_data = Telemetry()
                pod_data.ParseFromString(data)
                json_pod_data = json_format.MessageToJson(pod_data)
                sio.emit('pod_telemetry', json_pod_data)
                pod_data = MessageToDict(pod_data)
                log.debug("Telemetry: {}".format(pod_data))

    udp_socket.close()
    connection_status['status'] = 0
    sio.emit('connection_updates', json.dumps(connection_status))

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
