import socketio
import json
import time
from TelemetryLogger.LogToCSV import CsvTelemetryLoggerTesting


sio = socketio.Client()
csv_logger = None


@sio.on('connect')
def on_connect():
    print('Connected to Server')
    sio.emit('connected', 'CSV Telemetry Logger')


@sio.on('disconnect')
def on_disconnect():
    print('End Logging')
    global csv_logger
    csv_logger.end_session()


@sio.on('start_logging_session')
def start_logging_session(data):
    print('start Logging')
    global csv_logger
    csv_logger = CsvTelemetryLoggerTesting()
    csv_logger.start_log_session()
    sio.emit('logging_session_started', 'started')


@sio.on('end_logging_session')
def end_logging_session(data):
    print('End Logging')
    global csv_logger
    csv_logger.end_session()
    sio.emit('logging_session_ended', csv_logger.log_file_name)


@sio.on('telemetry')
def on_log_telemetry(data):
    print('logging data: {data}')
    global csv_logger
    telemetry = json.loads(data)
    try:
        csv_logger.log_telemetry(telemetry)
    except AttributeError or ValueError:
        print("Start logging session to capture telemetry")


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
