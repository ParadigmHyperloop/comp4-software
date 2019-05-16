from flask_socketio import SocketIO
from ControlLaptop.views import app

socket_io = SocketIO(app)


@socket_io.on('json')
def handle_json(message):
    print('received json: ' + message)


@socket_io.on('disconnect')
def handle_disconnect():
    print("someone left")


@socket_io.on('connect')
def print_connect():
    print('User Connected')


@socket_io.on('pds packet')
def pds_packet(packet):
    print('packet')
    socket_io.emit("pds packet", packet)


@socket_io.on('pod pulse')
def pod_pulse(pulse):
    print('pulse')
    socket_io.emit("pod pulse", pulse)


@socket_io.on('command')
def interface_command(command):
    print('command')
    socket_io.emit("command", command)


@socket_io.on('ping')
def pod_pulse(ping):
    print(ping)
    socket_io.emit("ping", ping)


@socket_io.on('telemetry_connection')
def telemetry_setup(status):
    print("Telemetry Connection:" + status)
    socket_io.emit("telemetry_connection", status)


@socket_io.on('telemetry')
def pod_telemetry(data):
    print("Transferring Telemetry")
    socket_io.emit("telemetry", data)


@socket_io.on('start_logging_session')
def emit_start_logging_session():
    socket_io.emit('start_logging_session')


@socket_io.on('end_logging_session')
def emit_end_logging_session():
    socket_io.emit('end_logging_session')


@socket_io.on('log_telemetry')
def emit_log_telemetry(data):
    socket_io.emit('log_telemetry', data)


@socket_io.on('logging_session_ended')
def emit_logging_file_saved(data):
    socket_io.emit('logging_session_ended', data)


@socket_io.on('logging_session_started')
def emit_logging_session_started(data):
    socket_io.emit('logging_session_started')
