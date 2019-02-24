import datetime
from influxdb import InfluxDBClient

class Database:

    def __init__(self, host, port, dbName):
        self.host = host
        self.port = port
        self.client = InfluxDBClient(self.host, self.port)
        self.client.create_database(dbName)
        self.client.switch_database(dbName)

    def log(self, name, data):
        row = [
            {
                "measurement": name,
                "time": datetime.datetime.now(),
                "fields": {
                    "reading": data
                }
            }
        ]
        self.client.write_points(row)

    def close(self):
        self.client.close()
