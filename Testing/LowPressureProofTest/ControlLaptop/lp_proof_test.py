from serial_manager import SerialManager
from database import Database
from csv_logger import CSVLogger
import sys


class TestStand:
    def __init__(self):
        self.serial_manager = SerialManager()
        try:
            self.database = Database()
        except KeyError:
            print('Error loading database')
            self.abort_test()
        self.csv_logger = CSVLogger()

    # create test log directories, select a serial port and begin the test
    def start_test(self):
        self.test_num = self.database.find_next_test_number()
        self.csv_logger.test_num = self.test_num
        print(f'\nStarting test {self.csv_logger.test_num}\n')
        self.csv_logger.make_test_data_dir()
        self.csv_logger.make_test_dir()

        print('Scanning serial ports\n')
        ports = self.serial_manager.get_available_serial_ports()
        if not ports:
            print('No serial ports were found')
            self.quit_test()
        else:
            # let user select the correct serial port
            print(('Choose a port from the options below.\n'
                   'Type the number of the port and press enter:'))
            for port in ports:
                print(ports.index(port)+1, ' - ', port)
            choice = input()
            self.port = ports[int(choice)-1]

        print(f'Press enter to start test {self.test_num}\n')
        input()
        self.run_test()

    # connect to the serial port, start the listener thread
    def run_test(self):
        self.serial_manager.open_port(self.port)
        print("Test started!")
        try:
            while True:
                # log serial telemetry to the influx database
                data = self.serial_manager.read_telemetry()
                if data:
                    data['test_number'] = self.test_num
                    self.database.log_data(data)
        except KeyboardInterrupt:
            self.finish_test()

    # allow the user to make notes about the test, then
    # log the test's notes and data inside it's directory
    def finish_test(self):
        print()
        print("Ending test")
        results = self.database.export_test(self.test_num)
        if results:
            self.csv_logger.log_as_csv(results)

    def abort_test(self):
        try:
            self.csv_logger.delete_test_files()
        except AttributeError:
            pass
        sys.exit()


if __name__ == '__main__':
    test_stand = TestStand()
    test_stand.start_test()
