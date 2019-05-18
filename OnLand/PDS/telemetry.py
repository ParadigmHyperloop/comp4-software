import socketio
import time
import logging as log
from datetime import timedelta
from PDS.config import SOCKET_SERVER, POD_IP, UDP_TELEM_PORT, MAX_MESSAGE_SIZE, UDP_TELEM_TIMEOUT
from PDS.UDP.PodUdpConnection import PodUdpConnection
from Paradigm_pb2 import telemetry
from google.protobuf import json_format
from google.protobuf.json_format import MessageToDict

log.basicConfig(filename='logs\telemetry.log', format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')


# Create socket to connect to server
sio = socketio.Client()


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
            sio.connect(SOCKET_SERVER)
        except:
            log.warning("Telemetry thread cannot connect to SocketIO")
            time.sleep(2)
        else:
            connected = True

    udp_socket = PodUdpConnection(POD_IP, UDP_TELEM_PORT, MAX_MESSAGE_SIZE)
    udp_socket.connect()
    if udp_socket.is_connected():
        sio.emit('telemetry_connection', '1')
    while udp_socket.is_connected():
        data = udp_socket.recv(timedelta(seconds=UDP_TELEM_TIMEOUT))
        if data is not None:
            pod_data = telemetry()
            pod_data.ParseFromString(data)
            print(pod_data.__str__());
            json_pod_data = json_format.MessageToJson(pod_data)
            sio.emit('telemetry', json_pod_data)
            pod_data = MessageToDict(pod_data)
            log.warning("Telemetry: {}".format(pod_data))

    udp_socket.close()
    sio.emit('telemetry_connection', '0')

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
