import socketio
import time
import logging as log
from datetime import timedelta
from PDS.config import SOCKET_SERVER, POD_IP, UDP_TELEM_PORT, UDP_RECV_BUFFER_SIZE, UDP_TELEM_TIMEOUT
from PDS.UDP.PodUdpConnection import PodUdpConnection
from Paradigm_pb2 import Telemetry
from google.protobuf import json_format
from google.protobuf.json_format import MessageToDict
import sys

log.basicConfig(stream=sys.stdout, format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p', level=log.INFO)


# Create socket to connect to server
sio = socketio.Client()

received = 0

@sio.on('connect')
def on_connect():
    log.info("Telemetry Thread : Connected")
    sio.emit('connected', "telemetry")


@sio.on('disconnect')
def on_disconnect():
    sio.emit('telemetry_connection', '0')
    log.info("Front End: Disconnected")


def main():
    log.info("Telemetry Thread Started")
    connected = False
    while not connected:
        try:
            sio.connect(SOCKET_SERVER)
        except:
            log.warning("Telemetry thread cannot connect to SocketIO")
            time.sleep(2)
        else:
            connected = True

    udp_socket = PodUdpConnection(POD_IP, UDP_TELEM_PORT, UDP_RECV_BUFFER_SIZE)
    udp_socket.connect()
    if udp_socket.is_connected():
        sio.emit('telemetry_connection', '1')
    while udp_socket.is_connected():
        data = udp_socket.recv(timedelta(seconds=UDP_TELEM_TIMEOUT))
        if data is not None:
            pod_data = Telemetry()
            pod_data.ParseFromString(data)
            json_pod_data = json_format.MessageToJson(pod_data)
            global received
            if received is 30:
                sio.emit('telemetry', json_pod_data)
                received = 0
            else:
                received = received + 1
            pod_data = MessageToDict(pod_data)
            log.debug("Telemetry: {}".format(pod_data))

    udp_socket.close()
    sio.emit('telemetry_connection', '0')

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
