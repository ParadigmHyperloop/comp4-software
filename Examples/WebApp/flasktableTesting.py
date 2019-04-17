from flask import Flask, render_template
from flask_table import Col, create_table
import datetime
from influxdb import *

app = Flask(__name__)


@app.route('/')
def index():
    def internalGenerator():
        # connect to db
        client = InfluxDBClient('localhost', 8086)
        client.switch_database('test')
        a = client.query("select last(*), min(*), max(*) from test.autogen.sensors group by id")

        for item in a.get_points():
            yield item['last_id_field']

    TableCls = create_table() \
        .add_column('sensor', Col('Sensors')) \
        .add_column('status', Col('OK')) \
        .add_column('min', Col('Min')) \
        .add_column('actual', Col('Actual')) \
        .add_column('max', Col('Max'))

    getSensors = internalGenerator()
    sensorsList = []
    for sensor in getSensors:
        sensorsList.append(dict(sensor=sensor, status='', min='', actual='', max=''))

    table = TableCls(sensorsList, html_attrs={'id':'sensorTab'} , no_items='Error', classes=['table table-hover '
                                                                                     'table-striped '
                                                                                     'table-condensed'])

    return render_template('sensorsTest.html', test=table)


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


if __name__ == '__main__':
    app.run()
