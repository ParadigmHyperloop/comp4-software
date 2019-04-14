from Paradigm_pb2 import *
import select
import time
import errno

import socket


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
            select.select([], [podSocket], [])  # This blocks until the whole message is sent
        except:
            print("Unexpected error on sending packet")
    return


# TCP_IP = '128.224.146.221'  # Beagle IP
TCP_IP = '127.0.0.1'  # Local IP
TCP_PORT = 5005
BUFFER_SIZE = 1024

PULSE_SPEED = 4000
BACKUP_PULSE = 5000
TIMEOUT_TIME = 6000

# Generate packet
podMessage = podCommand()
podMessage.controlsInterfaceState = ciFlight

connected = False

# Attempt to reconnect
while not connected:
    podSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        podSocket.connect((TCP_IP, TCP_PORT))
    except ConnectionRefusedError:
        # No connection keep going
        pass
    except:
        print("Unexpected Error on connect")
    else:
        podSocket.setblocking(False)
        connected = True

    lastPacket = current_time_milli()
    while connected:

        # Send Packet, non blocking sockets require a little extra magic to make sure the whole
        # packet gets sent.
        # Send a packets every 500 milliseconds.
        elapsed = current_time_milli() - lastPacket
        if elapsed > PULSE_SPEED:
            send_packet(podMessage.SerializeToString(), podSocket)

            # Receive Packet
            while elapsed > PULSE_SPEED and connected:
                try:
                    msg = podSocket.recv(BUFFER_SIZE)
                except BlockingIOError:
                    # When a non block socket doesnt receive anything it throws BlockingIOError
                    elapsed = current_time_milli() - lastPacket
                    if elapsed > BACKUP_PULSE:  # If we're getting close to timeout, send another one.
                        send_packet(podMessage.SerializeToString(), podSocket)
                    if elapsed > TIMEOUT_TIME:  # Heartbeat expired
                        print("Timeout")
                        connected = False
                except:  # Real Error
                    print("Error on reading packet")
                    connected = False
                else:  # No error, msg received
                    if not msg:  # Empty message means that the connection was terminated by the pod
                        print('Pod closed connection')
                        connected = False
                    else:
                        lastPacket = current_time_milli()
            elapsed = current_time_milli() - lastPacket

    # If we lose connection, close the socket and start another one.
    podSocket.close()




