from ControlLaptop.sockets import socket_io, app
import logging as log

log.basicConfig(filename='logs\controlLaptop.log', format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')


def main():
    log.warning("Heartbeat Thread Started")
    socket_io.run(app)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
