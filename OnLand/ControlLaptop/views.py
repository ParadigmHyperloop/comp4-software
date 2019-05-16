import logging as log
import json
import requests
from datetime import datetime
from flask import Flask, redirect, render_template, jsonify
from ControlLaptop.LocalStorage.ConfigurationSotrage import LocalStorage
from ControlLaptop.LocalStorage.FlightConfig import FlightConfig
from ControlLaptop.SocketController import PodCommunicator
from ControlLaptop.config import get_page_title, NAV_BAR
from ControlLaptop.forms import FlightConfigurationForm

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secrete-key'  # change later.

# Creating Flask Global PodCommunicator and connecting to Pod
with app.app_context():
    try:
        pod_communicator = PodCommunicator.get_pod_communicator()
    except Exception as e:
        log.info("COULD NOT CONNECT TO POD")

# --------------------------------------------------------------
# END SETUP
# --------------------------------------------------------------


@app.route("/")
def index():
    return redirect('/ui/')


@app.errorhandler(404)
def page_not_found(e):
    return render_template('404.html', title="Page Not Found"), 404


@app.context_processor
def inject_now():
    return {'now': datetime.utcnow(), 'navigation_bar': NAV_BAR}


@app.route('/submit_configuration', methods=['POST'])
def submit_configuration():
    configuration_form = FlightConfigurationForm()
    if configuration_form.validate_on_submit():
        FlightConfig.get_flight_config_instance().update_config(
            {
                'retrieval_timeout': int(configuration_form.retrieval_timeout.data),
                'max_flight_time': int(configuration_form.max_flight_time.data),
                'motor_speed': int(configuration_form.motor_speed.data),
                'telemetry_port': int(configuration_form.telemetry_port.data),
                'command_port': int(configuration_form.command_port.data),
                'flight_length': int(configuration_form.flight_Length.data),
                'heartbeat_timeout': int(configuration_form.heartbeat_timout.data),
                'pod_address': configuration_form.pod_ip.data,
                'pod_driver': 'Motor' if configuration_form.pod_driver.data is True else 'Simulation',
            }
        )
        command_sent = PodCommunicator.get_pod_communicator().send_configuration(
            configuration=FlightConfig.get_flight_config_instance().read_config())
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


@app.route('/dts')
def dts():
    page = 'dts'
    title = get_page_title(page)
    with open('ControlLaptop/LocalStorage/DtsSensors.json') as json_file:
        sensors = json.load(json_file)
    with open('ControlLaptop/LocalStorage/ElectricalValues.json') as json_file:
        electrical_sensors = json.load(json_file)
    return render_template(
        page+".html",
        active_page=page,
        title=title,
        sensors=sensors,
        electrical_senesors=electrical_sensors
    )


@app.route('/proofTest')
def proofTest():
    page = 'proofTest'
    title = get_page_title(page)
    with open('ControlLaptop/LocalStorage/ProofTestSensors.json') as json_file:
        sensors = json.load(json_file)
    return render_template(
        page+".html",
        active_page=page,
        title=title,
        sensors=sensors
    )


@app.route('/profile')
def get_flight_profile_template():
    page = 'profile'
    title = get_page_title(page)
    return render_template(
        page+".html",
        active_page=page,
        title=title,
        configuration_form=FlightConfigurationForm(),
        saved_configuration=FlightConfig.get_flight_config_instance().read_config()
    )


@app.route('/sensor_ranges')
def add_numbers():
    with open('ControlLaptop/LocalStorage/DtsSensors.json') as json_file:
        data = json_file.read().replace('\n', '')
    return data
