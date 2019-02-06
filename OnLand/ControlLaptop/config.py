from influxdb import InfluxDBClient
from flask_table import Col, create_table

DEFAULT_TITLE = "Control Interface"


# @TODO set db structure
# gets the name of the sensors from the db, used to generate sensor table
def db_query_sensor_names():
    # connect to db to get sensor names
    client = InfluxDBClient('localhost', 8086)
    client.switch_database('test')

    a = client.query("select last(*), min(*), max(*) from test.autogen.sensors group by id")

    for item in a.get_points():
        yield item['last_id_field']


def get_sensor_values():
    # connect to database
    client = InfluxDBClient('localhost', 8086)
    client.switch_database('test')

    all_sensors_name = db_query_sensor_names()

    for sensors in all_sensors_name:
        all_sensors_values = client.query(str(db_query_sensor_values(sensors)))

        for all_sensor_data in all_sensors_values:
            yield all_sensor_data[0]


def db_query_sensor_values(input):
    return str("select last(*), min(*), max(*) from test.autogen.sensors where id=" + "'" + input + "'")


def generate_sensor_table():
    # generates sensor table
    column = ['sensor', 'status', 'min', 'actual', 'max']

    sensor_table = create_table('sensors')
    for col in column:
        sensor_table.add_column(str(col), Col(str(col)))

    sensor_names = db_query_sensor_names()
    sensors_list = []
    for sensor in sensor_names:
        sensors_list.append(dict(sensor=sensor, status='', min='', actual='', max=''))

    table = sensor_table(sensors_list, html_attrs={'id': 'sensorTab'}, no_items='Error',
                         classes=['table table-hover table-striped table-condensed'])

    return table


NAV_BAR = [
    {
        'id': 'index',
        'href': 'index.html',
        'title': 'Vehicle Dashboard',
        'icon': 'pe-7s-graph'
    },
    {
        'id': 'battery',
        'href': 'battery.html',
        'title': 'Batteries',
        'icon': 'pe-7s-battery'
    },
    {
        'id': 'pid',
        'href': 'pid.html',
        'title': 'Air Supply',
        'icon': 'pe-7s-share'
    },
    {
        'id': 'profile',
        'href': 'profile.html',
        'title': 'Flight Profile',
        'icon': 'pe-7s-user'
    },
    {
        'id': 'overrides',
        'href': 'overrides.html',
        'title': 'Sensor Overrides',
        'icon': 'pe-7s-note2'
    },
    {
        'id': 'manual',
        'href': 'manual.html',
        'title': 'Manual Control',
        'icon': 'pe-7s-news-paper'
    },
    {
        'id': 'feeds',
        'href': 'feeds.html',
        'title': 'Live Streams',
        'icon': 'pe-7s-video'
    },
]
NAV_IDS = [x['id'] for x in NAV_BAR]
