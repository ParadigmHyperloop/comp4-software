from flask import Flask, render_template
from flask_socketio import SocketIO

app = Flask(__name__)
socket_io = SocketIO(app)


@app.route('/')
def landing_page():
    return render_template('DtsControl.html')

@socket_io.on('json')
def handle_json(message):
    print('received json: ' + message)


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


if __name__ == "__main__":
    socket_io.run(app, debug=True)

