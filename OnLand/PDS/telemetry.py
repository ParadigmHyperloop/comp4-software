import socketio
import time
import logging as log
import datetime
import socket
import socketio

from datetime import timedelta
from google.protobuf import json_format
from google.protobuf.json_format import MessageToDict

from PDS.spacexPacket import spacex_packet
from PDS.UDP.PodUdpConnection import PodUdpConnection
from PDS.helpers.heartbeat_timer import HeartbeatTimer
from Paradigm_pb2 import Telemetry
from config import SOCKET_SERVER, POD_IP, UDP_TELEM_PORT, MAX_MESSAGE_SIZE, UDP_TELEM_TIMEOUT, \
    TELEMETRY_BROADCAST_FREQUENCY, SPACEX_IP, SPACEX_PORT

import sys
import json
broadcast_timer = HeartbeatTimer()
log.basicConfig(stream=sys.stdout, format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')

# SpaceX Packet connection, only to be turned on during Competition run
SEND_SPACEX_PACKET = True
if SEND_SPACEX_PACKET:
    server = (SPACEX_IP, SPACEX_PORT)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    spacex_timer = HeartbeatTimer()

# Create socket to connect to server
sio = socketio.Client()
connection_status = {'name': 'telemetry', 'status': 0}

@sio.on('connect')
def on_connect():
    log.warning("Front End: Connected")
    sio.emit('connected', "telemetry")


@sio.on('disconnect')
def on_disconnect():
    connection_status['status'] = 0
    sio.emit('connection_updates', json.dumps(connection_status))
    sio.emit('telemetry_connection', '0')
    log.warning("Front End: Disconnected")


def main():
    log.warning("Telemetry Thread Started")
    connected = False
    start_time = None
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
            pod_data = Telemetry()
            pod_data.ParseFromString(data)

            if SEND_SPACEX_PACKET:
                if (MessageToDict(pod_data)['podState'] == 5) and (start_time == None):
                    start_time = datetime.datetime.now()
                if spacex_timer.time_since_pulse() >= 0.04 or pod_data.updateMessages:
                    spacex_timer.pulse()
                    packet = spacex_packet(MessageToDict(pod_data), start_time)
                    sock.sendto(packet, server)
            
            if broadcast_timer.time_since_pulse() > TELEMETRY_BROADCAST_FREQUENCY or pod_data.updateMessages:
                broadcast_timer.pulse()
                json_pod_data = json_format.MessageToJson(pod_data)
                sio.emit('pod_telemetry', json_pod_data)
                pod_data = MessageToDict(pod_data)
                log.warning("Telemetry: {}".format(pod_data))

        else:
            connection_status['status'] = 0
            sio.emit('connection_updates', json.dumps(connection_status))
            if SEND_SPACEX_PACKET:
                    packet = spacex_packet(None, None)
                    sock.sendto(packet, server)
            time.sleep(5)
    udp_socket.close()
    connection_status['status'] = 0
    sio.emit('connection_updates', json.dumps(connection_status))


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
