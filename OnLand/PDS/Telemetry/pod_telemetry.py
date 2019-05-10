import socketio
import logging
import time
from datetime import timedelta
from Telemetry.PodUdpConnection import PodUdpConnection
from PDS.config import *
from Paradigm_pb2 import *
from google.protobuf import json_format
from google.protobuf.json_format import MessageToDict

# Create socket to connect to server
sio = socketio.Client()
@sio.on('connect')
def on_connect():
    print("Telemetry Thread Connected to Server")
    sio.emit('connected', "telemetry")


@sio.on('disconnect')
def on_disconnect():
    sio.emit('telemetry_connection', '0')
    print('I\'m disconnected!')


def main():
    sio.connect(SOCKET_SERVER)
    udp_socket = PodUdpConnection(POD_IP, UDP_TELEM_PORT)
    udp_socket.connect()


    if udp_socket.is_connected():
        sio.emit('telemetry_connection', '1')
    while udp_socket.is_connected():
        data = udp_socket.recv(timedelta(seconds=UDP_TELEM_TIMEOUT))
        if data is not None:
            print('recvd')
            pod_data = telemetry()
            pod_data.ParseFromString(data)
            json_pod_data = json_format.MessageToJson(pod_data)
            print(pod_data.__str__())
            sio.emit('telemetry', json_pod_data)
            pod_data = MessageToDict(pod_data)
            logging.debug("Recv {}".format(pod_data))
            time.sleep(.2)
            print(pod_data.__str__())


    udp_socket.close()
    sio.emit('telemetry_connection', '0')

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
