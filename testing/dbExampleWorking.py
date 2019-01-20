import datetime
import random

from influxdb import *

list = [x for x in range(0, 100)]


def test(id, value):
    return [
        {
            "measurement": "sensors",
            "tags": {
                "id": id,
            },
            "time": datetime.datetime.now(),
            "fields": {
                "val": value,
                "id_field": id
            }
        }
    ]

def test1(name):
    for x in range(0,10):
        client.write_points(test(name, random.randrange(0,1000)))

def test2():
    while True:
        a = client.query("select last(*), min(*), max(*) from test.autogen.sensors where id='sensor1'")

        for item in a.get_points():
            print(item)


client = InfluxDBClient('localhost', 8086)

client.create_database("test")
client.switch_database('test')


# Take it out of the while loop to simply initialize, keep it in there to run in background
while True:
    test1('sensor1')
    test1('sensor2')
    test1('sensor3')
    test1('sensor4')
    test1('sensor5')
    test1('sensor6')



# a = client.query("select last(*), min(*), max(*) from test.autogen.sensors group by id")
a = client.query("select * from test.autogen.sensors group by id")

for item in a.get_points():
    print(item)


# def internalGenerator():
#     # connect to db to get sensor names
#     client = InfluxDBClient('localhost', 8086)
#     client.switch_database('test')
#     a = client.query("select last(*), min(*), max(*) from test.autogen.sensors group by id")
#
#     for item in a.get_points():
#         yield item['last_id_field']
#
#
# def reqFormat(input):
#     return str("select last(*), min(*), max(*) from test.autogen.sensors where id="+"'"+input+"'")
#
#
# def queryDB():
#     client = InfluxDBClient('localhost', 8086)
#     client.switch_database('test')
#
#     val = ['last_status', 'max_value', 'min_value', 'current_value']
#     a = internalGenerator()
#
#     for item in a:
#         a = client.query(str(reqFormat(item)))
#
#         for element in a:
#             print(element[0])
#
# queryDB()