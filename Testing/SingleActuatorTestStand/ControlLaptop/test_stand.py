import threading
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
            print('Error loading database. Did you set the TEST_STAND_DB '
                  'environment variable to the name of your InfluxDB database?')
            self.abort_test()
        self.csv_logger = CSVLogger()
        # thread for interpreting incoming serial data
        self.telemetry_thread = threading.Thread(target=self.telemetry_loop)
        self.thread_is_running = False

    # create test log directories, select a serial port and begin the test
    def start_test(self):
        self.test_num = self.database.find_next_test_number()
        self.csv_logger.test_num = self.test_num
        print(f'\nStarting test {self.test_num}\n')
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

        print(f'Ready to start test {self.test_num}\n')
        keyword = input('To begin type "start": ')
        while keyword != 'start':
            keyword = input('Wrong keyword. To begin type "start": ')
        self.run_test()

    # connect to the serial port, start the listener thread and allow the user
    # to control the solenoid with the enter/return key
    def run_test(self):
        self.serial_manager.open_port(self.port)
        self.thread_is_running = True
        self.telemetry_thread.start()
        print('Press enter to toggle solenoid')
        while True:
            if input() == 'q':
                print('\n')
                self.finish_test()
                break
            self.serial_manager.toggle_solenoid()

    # drive the solenoid low, allow the user to make notes about the test, then
    # log the test's notes and data inside it's directory
    def finish_test(self):
        if self.serial_manager.solenoid_state:
            self.serial_manager.toggle_solenoid()
            while self.serial_manager.solenoid_event_queued:
                pass
        self.thread_is_running = False
        notes = []
        note = input('Make any notes about this test below:\n')
        while note != '':
            notes.append(note)
            note = input()
        self.csv_logger.log_notes(notes)

        results = self.database.export_as_csv(self.test_num)
        if results:
            self.csv_logger.log_as_csv(results)

    def abort_test(self):
        self.thread_is_running = False
        try:
            self.csv_logger.delete_test_files()
        except AttributeError:
            pass
        sys.exit()

    def telemetry_loop(self):
        while self.thread_is_running:
            # if there is a toggle queued for the solenoid
            if self.serial_manager.solenoid_event_queued:
                # write a 0 or 1 to the serial port to toggle the solenoid
                self.serial_manager.serial.write(
                    str(self.serial_manager.solenoid_state).encode()
                )
                self.serial_manager.solenoid_event_queued = False
            # log serial telemetry to the influx database
            data = self.serial_manager.read_telemetry()
            if data:
                data['test_number'] = self.test_num
                self.database.log_data(data)


if __name__ == '__main__':
    test_stand = TestStand()
    test_stand.start_test()
