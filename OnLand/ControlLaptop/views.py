import collections
import json
import logging as log
import sys
from datetime import datetime

import requests
from flask import Flask, redirect, render_template, jsonify, request

from ControlLaptop.LocalStorage.ConfigurationSotrage import LocalStorage
from ControlLaptop.LocalStorage.FlightConfig import FlightConfig
from ControlLaptop.SocketController import PodCommunicator
from ControlLaptop.forms import FlightProfileForm, PodConfigurationForm
from ControlLaptop.templates._sidebar import get_page_title, NAV_BAR

log.basicConfig(stream=sys.stdout, format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p', level=log.INFO)
app = Flask(__name__)
app.config['SECRET_KEY'] = 'secrete-key'  # change later.

# Creating Flask Global PodCommunicator and connecting to Pod
with app.app_context():
    try:
        pod_communicator = PodCommunicator.get_pod_communicator()
    except Exception as e:
        log.info("Could not connect to pod")
    else:
        log.info("Connected to Config")


# --------------------------------------------------------------
# END SETUP
# --------------------------------------------------------------


@app.route("/")
def index():
    return redirect('/dashboard')


@app.errorhandler(404)
def page_not_found(e):
    return render_template('404.html', title="Page Not Found"), 404


@app.context_processor
def inject_now():
    return {'now': datetime.utcnow(), 'navigation_bar': NAV_BAR}


@app.route('/submit_configuration', methods=['POST'])
def submit_configuration():
    configuration_form = PodConfigurationForm()
    flight_configuration = FlightConfig.get_flight_config_instance()
    pod_communicator = PodCommunicator.get_pod_communicator()

    if configuration_form.validate_on_submit():
        flight_configuration.update_config(
            {
                'retrieval_timeout': int(configuration_form.retrieval_timeout.data),
                'telemetry_port': int(configuration_form.telemetry_port.data),
                'command_port': int(configuration_form.command_port.data),
                'heartbeat_timeout': int(configuration_form.heartbeat_timout.data),
                'pod_address': configuration_form.pod_ip.data,
                'pod_driver': 'Motor' if configuration_form.pod_driver.data is True else 'Simulation',
            }
        )
        command_sent = pod_communicator.send_configuration(configuration=flight_configuration.read_config())
        if command_sent is True:
            return jsonify({'status': 'ok'})
        else:
            return jsonify({'status': 'error', 'message': 'Failed to Connect'}), 418
    else:
        return jsonify({'error': configuration_form.errors})


@app.route('/send_command', methods=["POST"])
def send_command():
    command = requests.get_json()['command']
    try:
        pod_communicator.send_command(command)
    finally:
        return jsonify({'status': 'ok'})


@app.route('/disconnect_from_pod', methods=["POST"])
def disconnect_from_pod():
    pod_communicator.shutdown()
    return jsonify({'status': 'ok'})


@app.route("/ui/", defaults={'path': 'dashboard'})
@app.route("/ui/<path:path>")
def ui(path):
    title = get_page_title(path)
    return render_template(
        path+".html",
        active_page=path,
        title=title,
        sensors=[LocalStorage.get_sensors()],
    )


@app.route("/dashboard")
def dashboard():
    path = request.path
    title = get_page_title(path[1:])

    with open('ControlLaptop/templates/tables/BmsSensors.json') as json_file:
        bms_sensors = order_sensors(json.load(json_file))
    with open('ControlLaptop/templates/tables/InverterSensors.json') as json_file:
        inverter_sensors = order_sensors(json.load(json_file))

    arm_form = FlightProfileForm()
    return render_template(
        path+".html",
        active_page=path,
        title=title,
        armForm=arm_form,
        sensors=[LocalStorage.get_sensors()],
        bms_sensors=bms_sensors,
        inverter_sensors=inverter_sensors
    )


@app.route('/parseFlightProfile', methods=['post'])
def parse_flight_profile_command():
    form = FlightProfileForm()
    if form.validate_on_submit():
        from ControlLaptop.socketIoRoutes import socket_io as sio
        sio.emit("flight_profile_command", form.data, room="command_updates")
        return jsonify({'status': 'success'})
    return jsonify({'status': 'error', 'context': form.errors})


@app.route('/dts')
def dts():
    page = 'dts'
    title = get_page_title(page)
    with open('ControlLaptop/templates/tables/DtsSensors.json') as json_file:
        sensors = order_sensors(json.load(json_file))
    with open('ControlLaptop/templates/tables/BmsSensors.json') as json_file:
        bms_sensors = order_sensors(json.load(json_file))
    with open('ControlLaptop/templates/tables/InverterSensors.json') as json_file:
        inverter_sensors = order_sensors(json.load(json_file))
    return render_template(
        page+".html",
        active_page=page,
        title=title,
        sensors=sensors,
        bms_senesors=bms_sensors,
        inverter_sensors=inverter_sensors
    )


@app.route('/proofTest')
def proofTest():
    page = 'proofTest'
    title = get_page_title(page)
    with open('ControlLaptop/templates/tables/ProofTestSensors.json') as json_file:
        sensors = order_sensors(json.load(json_file))
    return render_template(
        page+".html",
        active_page=page,
        title=title,
        sensors=sensors
    )


@app.route('/profile')
def get_flight_profile_template():
    page = 'connect'
    title = get_page_title(page)
    return render_template(
        page+".html",
        active_page=page,
        title=title,
        configuration_form=PodConfigurationForm(),
        saved_configuration=FlightConfig.get_flight_config_instance().read_config()
    )


@app.route('/sensor_ranges')
def add_numbers():
    with open('ControlLaptop/templates/tables/SensorRanges.json') as json_file:
        data = json_file.read().replace('\n', '')
    return data


#   :(
def order_sensors(sensors):
    ordered_dict = collections.OrderedDict()
    for order in range(0, len(sensors)):
        for key, value in sensors.items():
            if value['order'] is order:
                ordered_dict[key] = value
                break
    return ordered_dict

