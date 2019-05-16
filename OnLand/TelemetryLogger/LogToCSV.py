from datetime import datetime
import csv


class CsvTelemetryLoggerTesting:
    log_file = None
    new_file = True
    csv_writer = None
    data_fields = ['podState', 'lp1', 'hp', 'sol1', 'sol2',
                   'pressureVesselTemperature', 'railTemperature']

    def __init__(self):
        self.log_file_name = 'log_' + datetime.now().strftime("%m-%d-%Y-%H_%M_%S") + '.csv'
        self.session_ended = False

    def start_log_session(self):
        self.log_file = open('TelemetryLogs/' + self.log_file_name, mode='w')
        self.csv_writer = csv.writer(self.log_file)

    def end_session(self):
        self.log_file.close()

    def log_telemetry(self, telem_json):
        if self.new_file is True:
            self.new_file = False
            self.csv_writer.writerow(telem_json.keys())
        self.csv_writer.writerow(telem_json.values())
