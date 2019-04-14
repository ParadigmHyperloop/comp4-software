import socketio
import json
from Paradigm_pb2 import *

# Globals
pod_message = podCommand()


# Create socket to connect to server
sio = socketio.Client()


@sio.on('connect')
def on_connect():
    print("Connected to Server")
    sio.emit('pds packet', "hello")


@sio.on('disconnect')
def on_disconnect():
    # Set the state to disconnected so that the pod knows?
    print('I\'m disconnected!')


@sio.on('command')
def on_command(json_string):
    command = json.loads(json_string)
    if command is 1:
        pod_message.manualBrakeNodeState = bnsBraking
    if command is 0:
        pod_message.manualBrakeNodeState = bnsStandby






sio.connect('http://localhost:5000')

while True:
    sio.emit()