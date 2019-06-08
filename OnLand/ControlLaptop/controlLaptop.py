from ControlLaptop.socketIoRoutes import socket_io, app
import logging as log

log.basicConfig(filename='logs\paradigm.log', format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')

def main():
    log.warning("Heartbeat Thread Started")
    socket_io.run(app, host='0.0.0.0', log_output=False)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
