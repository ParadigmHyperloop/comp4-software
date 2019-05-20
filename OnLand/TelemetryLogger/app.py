import socketio
import json
import time
from TelemetryLogger.LogToCSV import CsvTelemetryLoggerTesting
import logging as log
import sys

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


@sio.on('start_logging_session')
def start_logging_session(data):
    log.info('Start Logging')
    global csv_logger
    csv_logger = CsvTelemetryLoggerTesting()
    csv_logger.start_log_session()
    sio.emit('logging_session_started', 'started')


@sio.on('end_logging_session')
def end_logging_session(data):
    log.info('End Logging')
    global csv_logger
    csv_logger.end_session()
    sio.emit('logging_session_ended', csv_logger.log_file_name)


@sio.on('telemetry')
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
            sio.connect('http://localhost:5000')
        except:
            time.sleep(2)
        else:
            connected = True
    sio.wait()


if __name__ == '__main__':
    main()