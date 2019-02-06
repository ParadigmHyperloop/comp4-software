import datetime
import random

from influxdb import *

# Connects to Influx and creates db
client = InfluxDBClient('localhost', 8086)

client.create_database("test")
client.switch_database('test')

 
def dbData(id, value):
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


# for loop defines number of entries
# changes randrange for different values, or use random.choice(someList) to randomly chose from arbitrary list
def addToDB(name):
    for x in range(0,10):
        client.write_points(dbData(name, random.randrange(0,100)))

# change id value accordingly, but overall shouldn't be used
def retrieveDBData():
    # a = client.query("select last(*), min(*), max(*) from test.autogen.sensors group by id")
    a = client.query("select * from test.autogen.sensors group by id")

    for item in a.get_points():
        print(item)


# change parameter for different name
addToDB('sensor1')

# or use the following for simulated in real time
# while True:
#    addToDB('sensor1')

