#!/usr/bin/env python
import random
from datetime import datetime
from flask import *
from flask_table import Col, create_table
from influxdb import InfluxDBClient

DEFAULT_TITLE = "Control Interface"


app = Flask(__name__)


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


# gets the name of the sensors from the db, used to generate sensor table
def sensorNameGenerator():
    # connect to db to get sensor names
    client = InfluxDBClient('localhost', 8086)
    client.switch_database('test')
    a = client.query("select last(*), min(*), max(*) from test.autogen.sensors group by id")

    for item in a.get_points():
        yield item['last_id_field']


@app.context_processor
def inject_now():
    return {'now': datetime.utcnow(),
            'navigation_bar': NAV_BAR}


@app.route("/ui/", defaults={'path': 'index.html'})
@app.route("/ui/<path:path>")
def ui(path):
    # generates sensor table
    column = ['sensor', 'status', 'min', 'actual', 'max']

    sensorTable= create_table('sensors')
    for col in column:
        sensorTable.add_column(str(col), Col(str(col)))

    getSensors = sensorNameGenerator()
    sensorsList = []
    for sensor in getSensors:
        sensorsList.append(dict(sensor=sensor, status='', min='', actual='', max=''))

    table = sensorTable(sensorsList, html_attrs={'id': 'sensorTab'}, no_items='Error',
                        classes=['table table-hover table-striped table-condensed'])

    # generates the side bar
    page = path.split('.')[0]
    if page in NAV_IDS:
        title = NAV_BAR[NAV_IDS.index(page)]['title']
    else:
        title = DEFAULT_TITLE
    return render_template(path, active_page=page, title=title, test=table)


@app.errorhandler(404)
def page_not_found(e):
    return render_template('404.html', title="Page Not Found"), 404


@app.route("/")
def index():
    return redirect('/ui/')


# TODO change, implement refresh from influxDB
@app.route("/_refreshTables")
def getAirSupply():
    status = ['0','1']
    values = []
    for x in range(0,100):
        values.append(x)

    return jsonify(value00=random.choice(values), value01=random.choice(values), value02=random.choice(values), status0=random.choice(status),
                   value10=random.choice(values), value11=random.choice(values), value12=random.choice(values), status1=random.choice(status),
                   value20=random.choice(values), value21=random.choice(values), value22=random.choice(values), status2=random.choice(status),
                   value30=random.choice(values), value31=random.choice(values), value32=random.choice(values), status3=random.choice(status),
                   )


def reqFormat(input):
    return str("select last(*), min(*), max(*) from test.autogen.sensors where id="+"'"+input+"'")


def sensorDataGenerator():
    client = InfluxDBClient('localhost', 8086)
    client.switch_database('test')

    a = sensorNameGenerator()

    for item in a:
        a = client.query(str(reqFormat(item)))

        for element in a:
            yield element[0]


# TODO change, implement refresh from influxDB
@app.route("/_sensorsRefresh")
def queryDB():
    test = sensorDataGenerator()
    return jsonify(tuple(test))


if __name__ == '__main__':
    app.run()
