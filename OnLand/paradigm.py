from ControlLaptop.controlLaptop import main as app
from PDS.telemetry import main as telemetry
from PDS.commander import main as heartbeat
from multiprocess as m
import logging as log

log.basicConfig(filename='logs\paradigm.log', format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')


def main():
    """
    threads = [Thread(target=app), Thread(target=telemetry), Thread(target=heartbeat)]

    for t in threads:
        t.start()

    for t in threads:
        t.join()

    print("All threads haulted")
    """
    print("woohoo")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
