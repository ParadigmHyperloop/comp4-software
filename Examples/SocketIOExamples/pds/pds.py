import socketio
from Paradigm_pb2 import *
import select
import time
import socket

# Globals

podMessage = podCommand()
podMessage.controlsInterfaceState = ciFlight
# TCP_IP = '128.224.146.221'  # Beagle IP
TCP_IP = '127.0.0.1'  # Local IP
TCP_PORT = 5005
BUFFER_SIZE = 1024
PULSE_SPEED = 1000
BACKUP_PULSE = 1500
TIMEOUT_TIME = 2000
last_packet_time = 0

# Create socket to connect to server
sio = socketio.Client()
@sio.on('connect')
def on_connect():
    print("Connected to Server")
    sio.emit('connected', "pds")


@sio.on('disconnect')
def on_disconnect():
    # Set the state to disconnected so that the pod knows?
    print('I\'m disconnected!')


@sio.on('command')
def on_command(command):
    if command is '1':
        podMessage.manualBrakeNodeState = bnsBraking
    if command is '0':
        podMessage.manualBrakeNodeState = bnsStandby


sio.connect('http://localhost:5000')


# PDS functions
def current_time_milli():
    return int(round(time.time() * 1000))


def send_packet(payload, sock):
    total_sent = 0
    while len(payload):
        try:
            sent = sock.send(payload)
            total_sent += sent
            payload = payload[sent:]
        except socket.EAGAIN:
            select.select([], [sock], [])  # This blocks until the whole message is sent
        except:
            print("Unexpected error on sending packet")
    return


def time_since_last_packet():
    time_elapsed = current_time_milli() - last_packet_time
    return time_elapsed


def connect_to_pod(ip_addr, port):
    connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        connection.connect((ip_addr, port))
    except ConnectionRefusedError:
        # No connection
        return False
        pass
    except:
        print("Unexpected Error on connect")
    else:
        connection.setblocking(False)
        return connection


connected = False
while not connected:
    pod_socket = connect_to_pod(TCP_IP, TCP_PORT)
    if pod_socket:
        connected = True
    else:
        time.sleep(2)

last_packet_time = current_time_milli()
while connected:

    # Send Packet, non blocking sockets require a little extra magic to make sure the whole
    # packet gets sent.
    # Send a packets every 500 milliseconds.
    if time_since_last_packet() > PULSE_SPEED:
        send_packet(podMessage.SerializeToString(), pod_socket)

        # Receive Packet
        while time_since_last_packet() > PULSE_SPEED and connected:
            try:
                msg = pod_socket.recv(BUFFER_SIZE)
            except BlockingIOError:
                # When a non block socket doesnt receive anything it throws BlockingIOError
                if time_since_last_packet() > BACKUP_PULSE:  # If we're getting close to timeout, send another one.
                    send_packet(podMessage.SerializeToString(), pod_socket)
                if time_since_last_packet() > TIMEOUT_TIME:  # Heartbeat expired
                    print("Timeout")
                    connected = False
            # except:  # Real Error
            #     print("Error on reading packet")
            #     connected = False
            else:  # No error, msg received
                if not msg:  # Empty message means that the connection was terminated by the pod
                    print('Pod closed connection')
                    connected = False
                else:
                    sio.emit('ping', '1')
                    last_packet_time = current_time_milli()

# If we lose connection, close the socket and start another one.
sio.emit('ping', '0')
print("ping 0")
pod_socket.close()
