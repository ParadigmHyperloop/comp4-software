from flask_socketio import SocketIO
from ControlLaptop.views import app
import logging as log


socket_io = SocketIO(app)


@socket_io.on('json')
def handle_json(message):
    log.debug('received json: ' + message)


@socket_io.on('disconnect')
def handle_disconnect():
    log.debug("someone left")


@socket_io.on('connect')
def print_connect():
    log.debug('User Connected')


@socket_io.on('command')
def interface_command(command):
    log.debug('command')
    socket_io.emit("command", command)


@socket_io.on('ping', namespace='/commander')
def pod_ping(ping):
    log.debug(ping)
    socket_io.emit("ping", ping, namespace='/controls-interface')


@socket_io.on('telemetry_connection')
def telemetry_setup(status):
    log.debug("Telemetry Connection:" + status)
    socket_io.emit("telemetry_connection", status)


@socket_io.on('telemetry')
def pod_telemetry(data):
    log.debug("Transferring Telemetry")
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
