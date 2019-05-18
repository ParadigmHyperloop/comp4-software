from ControlLaptop.controlLaptop import main as app
from telemetry import main as telemetry
from commander import main as heartbeat
from threading import Thread
import logging as log

log.basicConfig(filename='logs\paradigm.log', format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')


def main():
    threads = [Thread(target=app), Thread(target=telemetry), Thread(target=heartbeat)]

    for t in threads:
        t.start()

    for t in threads:
        t.join()

    print("All threads haulted")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
