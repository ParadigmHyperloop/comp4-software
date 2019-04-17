import datetime
from influxdb import InfluxDBClient

class Database:

    def __init__(self, host, port, dbName):
        self.host = host
        self.port = port
        self.client = InfluxDBClient(self.host, self.port)
        self.dbName = dbName

        try:
            self.client.create_database(dbName)
            print("Database " + dbName + " created...")
        except InfluxDBClientError:
            self.client.drop_database(dbName)
            self.client.create_database(dbName)

    def log(self, measurement, data):
        row = [
            {
                "measurement": measurement,
                "time": datetime.datetime.now(),
                "fields": {
                    "reading": data
                }
            }
        ]
        self.client.write_points(row, database=self.dbName)

    def close(self):
        self.client.close()
