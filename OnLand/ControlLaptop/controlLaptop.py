from ControlLaptop.sockets import socket_io, app


def main():
    socket_io.run(app)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
