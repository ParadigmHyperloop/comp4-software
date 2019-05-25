from ControlLaptop.controlLaptop import main as web_app
from PDS.telemetry import main as telemetry
from PDS.commander import main as heartbeat
from multiprocessing import Process
import logging as log
import sys

log.basicConfig(stream=sys.stdout, format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')


def main():
    processes = [Process(target=web_app), Process(target=telemetry), Process(target=heartbeat)]

    for p in processes:
        p.start()

    for p in processes:
        p.join()

    print("All threads haulted")



if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
