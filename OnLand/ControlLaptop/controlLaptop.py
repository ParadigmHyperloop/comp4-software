#!/home/lwaghorn/Development/comp4-software/OnLand/venv/bin/python3

from sockets import socket_io, app

if __name__ == '__main__':
    socket_io.run(app, debug=True)
