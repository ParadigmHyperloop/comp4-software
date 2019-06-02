import socketio
import json
import time
from TelemetryLogger.LogToCSV import CsvTelemetryLoggerTesting
import logging as log
import sys
from config import SOCKET_SERVER

log.basicConfig(stream=sys.stdout, format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')
sio = socketio.Client()
csv_logger = None


@sio.on('connect')
def on_connect():
    log.info('Logger Thread : Connected to Server')
    sio.emit('connected', 'CSV Telemetry Logger')


@sio.on('disconnect')
def on_disconnect():
    log.info('End Logging')
    global csv_logger
    if csv_logger is not None:
        csv_logger.end_session()


@sio.on('logger_control')
def parse_command(command_json):
    global csv_logger
    command_object = json.loads(command_json)
    command = command_object['command']

    response = dict()
    if command == "start":
        csv_logger = CsvTelemetryLoggerTesting()
        csv_logger.start_log_session()
        response['feedback'] = 'started'
    elif command == "stop":
        if csv_logger is not None:
            csv_logger.end_session()
            response['feedback'] = 'stopped'
            response['filename'] = csv_logger.log_file_name
    sio.emit('logger_feedback', json.dumps(response))


@sio.on('pod_telemetry')
def on_log_telemetry(data):
    global csv_logger
    telemetry = json.loads(data)
    try:
        csv_logger.log_telemetry(telemetry)
    except:
        log.debug("Start logging session to capture telemetry")


def main():
    connected = False
    while not connected:
        try:
            sio.connect(SOCKET_SERVER)
        except:
            time.sleep(2)
        else:
            connected = True
    sio.emit('join_room', 'logger_control_updates')
    sio.emit('join_room', 'telemetry_updates')
    sio.wait()


if __name__ == '__main__':
    main()
