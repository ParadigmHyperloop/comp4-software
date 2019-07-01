from datetime import datetime
import time
import csv


class CsvTelemetryLoggerTesting:
    log_file = None
    new_file = True
    csv_writer = None
    _start_time = time.time()

    def __init__(self):
        self.log_file_name = 'log_' + datetime.now().strftime("%m-%d-%Y-%H_%M_%S") + '.csv'
        self.session_ended = False

    def start_log_session(self):
        self._start_time = time.time()
        self.log_file = open('TelemetryLogger/csv/' + self.log_file_name, mode='w+')
        self.csv_writer = csv.writer(self.log_file)

    def end_session(self):
        if self.log_file is not None:
            self.log_file.close()
            self.log_file = None

    def log_telemetry(self, telem_json):
        data = self._format_json(telem_json)
        if self.new_file is True:
            self.new_file = False
            self.csv_writer.writerow(data.keys())
        self.csv_writer.writerow(data.values())

    def _format_json(self, json):
        formatted = dict()
        formatted['Time'] = int(time.time() - self._start_time)
        formatted['Brake Node State'] = json['brakeNodeState']
        formatted['highPressure'] = json['highPressure']
        formatted['Module 1 Pressure'] = round(json['lowPressure1'], 2)
        formatted['Module 2 Pressure'] = round(json['lowPressure2'], 2)
        formatted['Module 3 Pressure'] = round(json['lowPressure3'], 2)
        formatted['Regulated Pressure'] = round(json['lowPressure4'], 2)
        formatted['High Pressure'] = round(json['highPressure'], 2)
        formatted['Tank Temperature'] = round(json['pressureVesselTemperature'], 2)
        formatted['Solenoid 1'] = json['solenoid1']
        formatted['Solenoid 2'] = json['solenoid2']
        formatted['Solenoid 3'] = json['solenoid3']
        formatted['Solenoid 4'] = json['solenoid4']
        formatted['podState'] = json['podState']
        formatted['motorSpeed'] = json['motorSpeed']
        formatted['motorTorque'] = json['motorTorque']
        formatted['maxFlightTime'] = json['maxFlightTime']
        formatted['irRpm'] = json['irRpm']
        formatted['irDistance'] = json['irDistance']
        return formatted
