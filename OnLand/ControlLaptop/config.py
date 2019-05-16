from influxdb import InfluxDBClient


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
    # TODO: Move this to ODS. Updated design does not connect to DB
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



NAV_BAR = [
    {
        'id': 'dashboard',
        'href': 'ui/dashboard',
        'title': 'Vehicle Dashboard',
        'icon': 'pe-7s-rocket'
    },
    {
        'id': 'dts',
        'href': 'dts',
        'title': 'Dynamic Test Stand',
        'icon': 'pe-7s-joy'
    },
    {
        'id': 'proofTest',
        'href': 'proofTest',
        'title': 'High Pressure Proof Test',
        'icon': 'pe-7s-joy'
    },
    {
        'id': 'battery',
        'href': 'ui/battery',
        'title': 'Batteries',
        'icon': 'pe-7s-battery'
    },
    {
        'id': 'profile',
        'href': 'profile',
        'title': 'Flight Profile',
        'icon': 'pe-7s-user'
    },
    {
        'id': 'feeds',
        'href': 'ui/feeds',
        'title': 'Live Streams',
        'icon': 'pe-7s-video'
    },
]
NAV_IDS = [x['id'] for x in NAV_BAR]


def get_page_title(page):
    if page in NAV_IDS:
        title = NAV_BAR[NAV_IDS.index(page)]['title']
    else:
        title = DEFAULT_TITLE
    return title