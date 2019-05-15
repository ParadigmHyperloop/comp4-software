from datetime import datetime
import csv


class CsvTelemetryLoggerTesting:
    log_file = None
    new_file = True
    csv_writer = None
    data_fields = ['Pod State', 'lp1', 'hp', 'sol1', 'sol2',
                   'Pressure Vessel Temperature', 'Rail Temperature']

    def __init__(self):
        self.log_file_name = 'log_' + datetime.now().strftime("%m-%d-%Y-%H_%M_%S") + '.csv'
        self.session_ended = False

    def start_log_session(self):
        self.log_file = open('TelemetryLogs/' + self.log_file_name, mode='w')
        self.csv_writer = csv.DictWriter(self.log_file,
                                         fieldnames=self.data_fields)

    def end_session(self):
        self.log_file.close()

    def log_telemetry(self, telem_json):
        if self.new_file is True:
            self.new_file = False
            self.csv_writer.writeheader()
        self.csv_writer.writerow(telem_json)
