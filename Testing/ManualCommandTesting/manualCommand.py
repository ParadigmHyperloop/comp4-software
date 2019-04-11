from Paradigm_pb2 import podCommand, ControlsInterfaceStates
import select
import time
import errno

import socket

milliseconds = int(round(time.time() * 1000))
#TCP_IP = '128.224.146.221'
TCP_IP = '127.0.0.1'
TCP_PORT = 5005
BUFFER_SIZE = 1024

# Generate packet
podMessage = podCommand()
podMessage.controlsInterfaceState = ControlsInterfaceStates.ciFlight

podSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
podSocket.connect((TCP_IP, TCP_PORT))
podSocket.setblocking(False)
connected = True

lastPacket = milliseconds
while connected:

    # Send Packet
    elapsed = milliseconds - lastPacket
    print(elapsed)
    if elapsed > 200:
        total_sent = 0
        payload = podMessage.SerializeToString()
        print(payload)
        payload_size = len(payload)
        while len(payload):
            try:
                sent = podSocket.send(payload)
                total_sent += sent
                payload = payload[sent:]
            except socket.error as e:
                if e.errno != errno.EAGAIN:
                    raise e
                select.select([], [podSocket], [])  # This blocks until

    # Receive Packet
    try:
        msg = podSocket.recv(4096)
    except socket.error as e:
        err = e.args[0]
        if err == errno.EAGAIN or err == errno.EWOULDBLOCK:
            # No data received
            elapsed = milliseconds - lastPacket
            print(elapsed)
            if elapsed > 1000:
                print("Timeout")
                connected = False
        else:
            # Real Error
            connected = False
    else:
        if not msg:
            print('orderly shutdown on server end')
        else:
            lastPacket = milliseconds

podSocket.close()




