import socketio
import logging
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
    sio.emit('connected', "telem")


@sio.on('disconnect')
def on_disconnect():
    print('I\'m disconnected!')


def main():
    sio.connect(SOCKET_SERVER)
    pod = PodUdpConnection(POD_IP, UDP_TELEM_PORT)
    pod.connect()

    while pod.is_connected():
        data = pod.recv(timedelta(seconds=UDP_TELEM_TIMEOUT))
        if data is not None:
            pod_data = telemetry()
            pod_data.ParseFromString(data)
            json_pod_data = json_format.MessageToJson(pod_data)
            sio.emit('telemetry', json_pod_data)

            pod_data = MessageToDict(pod_data)
            logging.debug("Recv {}".format(pod_data))

    pod.close()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
