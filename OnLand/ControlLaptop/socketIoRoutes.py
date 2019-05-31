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


@socket_io.on('command', namespace='/telemetry-subscribers')
def interface_command(command):
    log.debug('command')
    socket_io.emit("command", command, namespace='/command-subscribers')


@socket_io.on('connection_updates')
def pod_connection_updates(update):
    log.debug(update)
    socket_io.emit("connection_update", update, room='connection_updates', namespace='/controls-interface')


@socket_io.on('pod_telemetry')
def pod_telemetry(data):
    log.debug("Transferring Telemetry")
    socket_io.emit("pod_telemetry", data, namespace='/telemetry-subscribers')


@socket_io.on('logger_control')
def logger_controls():
    socket_io.emit('logger_control', namespace='/logger_controls')


@socket_io.on('logger_feedback')
def logger_feedback(feedback):
    socket_io.emit('logger_feedback', feedback, namespace='/telemetry-subscribers')

