from ControlLaptop.controlLaptop import main as web_app
from PDS.telemetry import main as telemetry
from config import SOCKET_SERVER
from PDS.commander import main as heartbeat
from multiprocessing import Process
import logging as log
import sys
import time
import socketio

log.basicConfig(stream=sys.stdout, format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')

# Create socket to connect to server
sio = socketio.Client()
processes = list()


@sio.on('connect')
def on_connect():
    log.warning("Process Control: Connected")
    sio.emit('connected', "process control")


@sio.on('disconnect')
def on_disconnect():
    sio.emit('Process Control', '0')
    log.warning("Process Control: Disconnected")


@sio.on('cycle_process')
def cycle_process(data):
    index = int(data)
    processes[index].terminate()
    if index is 0:
        new_process = Process(target=web_app)
    elif index is 1:
        new_process = Process(target=telemetry)
    elif index is 2:
        new_process = Process(target=heartbeat)
    else:
        return
    processes[index] = new_process
    processes[index].start()


def main():
    global processes
    processes = [Process(target=web_app), Process(target=telemetry), Process(target=heartbeat)]

    for p in processes:
        p.start()

    connected = False
    while not connected:
        try:
            sio.connect(SOCKET_SERVER)
        except:
            log.info("Process Control cannot connect to SocketIO")
            time.sleep(2)
        else:
            connected = True
            sio.emit('join_room', 'process_control')

    for p in processes:
        p.join()

    print("All threads haulted")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
