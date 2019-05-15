import socketio
from TelemetryLogger.LogToCSV import CSVTelemetryLogger

sio = socketio.Client()
csv_logger = None


@sio.on('start_logging_session')
def start_logging_session():
    csv_logger = CSVTelemetryLogger()
