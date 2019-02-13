import datetime
import os
from influxdb import InfluxDBClient


class Database:
    def __init__(self):
        self.client = InfluxDBClient(host='localhost', port=8086)
        self.client.create_database(os.environ['TEST_STAND_DB'])
        self.client.switch_database(os.environ['TEST_STAND_DB'])

    # logs test data to the database
    def log_data(self, data):
        table_row = [{
            'measurement': 'sensor_data',
            'time': datetime.datetime.now(),
            'fields': data
        }]
        self.client.write_points(table_row)

    def export_as_csv(self, test_number):
        results = self.client.query(
            f'SELECT * FROM "satsdb".."sensor_data" WHERE test_number = {test_number}'
        )
        try:
            return (results.raw['series'][0])
        except KeyError:
            return

    def find_next_test_number(self):
        results = self.client.query(
            'SELECT test_number FROM "satsdb".."sensor_data"'
        )
        try:
            results = [index[1] for index in results.raw['series'][0]['values']]
        except KeyError:
            self.test_number = 1
        else:
            self.test_number = max(results)+1
        return self.test_number
