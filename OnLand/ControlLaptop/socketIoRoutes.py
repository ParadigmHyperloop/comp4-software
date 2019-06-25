from flask_socketio import SocketIO, join_room
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


@socket_io.on('join_room')
def complete_room_join(room):
    join_room(room)


@socket_io.on('frontend_notification')
def frontend_notification(notification):
    socket_io.emit("frontend_notification", notification, room="notification_updates")


@socket_io.on('connection_updates')
def pod_connection_updates(update):
    socket_io.emit("connection_update", update, room='connection_updates')


@socket_io.on('pod_telemetry')
def pod_telemetry(data):
    socket_io.emit("pod_telemetry", data, room='telemetry_updates')


@socket_io.on('manual_state_command')
def manual_command(command):
    socket_io.emit("manual_state_command", command, room='command_updates')


@socket_io.on('manual_configuration_command')
def manual_configuration_command(command):
    socket_io.emit("manual_configuration_command", command, room='command_updates')


@socket_io.on('logger_control')
def logger_controls(data):
    socket_io.emit('logger_control', data, room='logger_control_updates')


@socket_io.on('logger_feedback')
def logger_feedback(feedback):
    socket_io.emit('logger_feedback', feedback, room='logger_feedback_updates')


@socket_io.on('cycle_process')
def logger_feedback(feedback):
    socket_io.emit('cycle_process', feedback, room='process_control')

