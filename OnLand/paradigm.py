from ControlLaptop.controlLaptop import main as app
from PDS.telemetry import main as telemetry
from PDS.heartbeat import main as heartbeat
from threading import Thread


def main():
    web_app = Thread(target=app)
    telem = Thread(target=telemetry)
    hb = Thread(target=heartbeat)

    web_app.start()
    telem.start()
    hb.start()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
