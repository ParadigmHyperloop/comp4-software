import datetime
import os
from influxdb import InfluxDBClient


class Database:
    def __init__(self):
        self.client = InfluxDBClient(host='localhost', port=8086)
        self.client.create_database("lp_db")
        self.client.switch_database("lp_db")

    # find the next available number to identify a test
    def find_next_test_number(self):
        results = self.client.query(
            'SELECT test_number FROM "lp_db".."sensor_data"'
        )
        try:
            results = [index[1] for index in results.raw['series'][0]['values']]
        except KeyError:
            self.test_number = 1
        else:
            self.test_number = max(results)+1
        return self.test_number

    # logs a single row to the database
    def log_data(self, data):
        table_row = [{
            'measurement': 'sensor_data',
            'time': datetime.datetime.now(),
            'fields': data
        }]
        self.client.write_points(table_row)

    # pulls all data from a certain test and returns it
    def export_test(self, test_number):
        results = self.client.query(
            f'SELECT * FROM "lp_db".."sensor_data" WHERE test_number = {test_number}'
        )
        try:
            return (results.raw['series'][0])
        except KeyError:
            return
