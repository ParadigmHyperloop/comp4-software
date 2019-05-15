from datetime import datetime
import csv


class CSVTelemetryLogger:
    log_file = None
    new_file = True
    csv_writer = None

    def __init__(self):
        self.log_file_name = 'log' + datetime.now().strftime("%m/%d/%Y, %H:%M:%S")
        self.session_ended = False

    def start_log_session(self):
        self.log_file = open('TelemetryLogs/' + self.log_file_name, mode='w')
        self.csv_writer = csv.writer(self.log_file, delimiter=',', quotechar='"')

    def end_session(self):
        self.log_file.close()

    def log_telemetry(self, telem_json):
        if self.new_file is True:
            self.new_file = False
            self.csv_writer.writerow()
        else:
            #log values
