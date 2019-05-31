import socketio
import json
import time
from TelemetryLogger.LogToCSV import CsvTelemetryLoggerTesting
import logging as log
import sys
from config import SOCKET_SERVER

log.basicConfig(stream=sys.stdout, format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p', level=log.INFO)
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
    csv_logger.end_session()


@sio.on('logger_control')
def parse_command(command_json):
    global csv_logger
    command_object = json.loads(command_json)
    command = command_object['command']

    if command is 'start':
        csv_logger = CsvTelemetryLoggerTesting()
        csv_logger.start_log_session()
        sio.emit('logger_feedback', {'feedback': 'started'})
    elif command is 'stop':
        csv_logger.end_session()
        sio.emit('logger_feedback', {'feedback': 'stopped', 'filename': csv_logger.log_file_name})


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
            sio.connect(SOCKET_SERVER, namespaces=['/logger_controls', '/telemetry_subscribers'])
        except:
            time.sleep(2)
        else:
            connected = True
    sio.wait()


if __name__ == '__main__':
    main()
