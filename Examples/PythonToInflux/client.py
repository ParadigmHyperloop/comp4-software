import argparse
from influxdb import InfluxDBClient
import datetime

"""This file is not needed anymore"""

def main(host='localhost', port=8086):
    """Instantiate a connection to the InfluxDB."""
    user = 'root'
    password = 'root'
    dbname = 'test2'

    client = InfluxDBClient(host, port, user, password, dbname)

    print("Create database: " + dbname)
    client.create_database(dbname)
    client.switch_database(dbname)

    print("Create a retention policy")
    client.create_retention_policy('awesome_policy', '3d', 3, default=True)


    for x in range(20):
        data = point("sensor_data", "some-tag", x, "some-field", "some-field-data")
        print("Write points: {0}".format(data))
        client.write_points(data)

    client.close()
    #
    # print("Drop database: " + dbname)
    # client.drop_database(dbname)

def point(measurement, tag, tag_data, field, field_data):

    json_body = [
        {
            "measurement": measurement,
            "tags": {
                tag: tag_data,
            },
            "time": datetime.datetime.now(),
            "fields": {
                field: field_data
            }
        }
    ]
    return json_body




def parse_args():
    """Parse the args."""
    parser = argparse.ArgumentParser(
        description='example code to play with InfluxDB')
    parser.add_argument('--host', type=str, required=False,
                        default='localhost',
                        help='hostname of InfluxDB http API')
    parser.add_argument('--port', type=int, required=False, default=8086,
                        help='port of InfluxDB http API')
    return parser.parse_args()


if __name__ == '__main__':
    args = parse_args()
    main(host=args.host, port=args.port)
