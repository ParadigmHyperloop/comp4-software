import sys
import glob
import serial


class SerialManager:
    def __init__(self):
        self.solenoid_event_queued = False
        self.solenoid_state = 0

    # sets the 'self.solenoid_event_queued' flag,
    # signalling the telemetry thread to toggle the solenoid valve
    def toggle_solenoid(self):
        if self.solenoid_state:
            self.solenoid_state = 0
        else:
            self.solenoid_state = 1
        self.solenoid_event_queued = True

    def choose_port(self):
        ports = self.get_available_serial_ports()
        if len(ports) == 0:
            print('No serial ports were found.')
        elif len(ports) == 1:
            return ports[0]
        else:
            print(('Choose a port from the options below. '
                   'Type the number of the port and press enter:'))
            for port in self.serial_ports():
                print(ports.index(port)+1, '   ' + port)
            choice = input()
            return ports[int(choice)-1]

    def get_available_serial_ports(self):
        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux'):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob('/dev/tty[A-Za-z]*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')
        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        return result

    def open_port(self, port):
        self.serial = serial.Serial(
            port=port,
            baudrate=9600,
            timeout=1
        )

    def read_telemetry(self):
        telemetry = self.serial.readline()
        if not telemetry:
            return
        telemetry = str(telemetry)
        telemetry = telemetry.split(",")
        telemetry[0] = telemetry[0][2:]
        del telemetry[-1]
        try:
            data = {
                'milliseconds': float(telemetry[0]),
                'tank_pressure': float(telemetry[1]),
                'piston_pressure': float(telemetry[2]),
                'temperature_1': float(telemetry[3]),
                'temperature_2': float(telemetry[4]),
                'load': float(telemetry[5]),
                'solenoid_driven': telemetry[6]
            }
        except (IndexError, ValueError):
            return
        except ValueError:
            return
        return data
