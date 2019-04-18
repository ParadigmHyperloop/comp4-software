from datetime import datetime
from flask import *

from LocalStorage.ConfigurationSotrage import LocalStorage
from SocketController import PodCommunicator
from config import *
from forms import FlightConfigurationForm, validate_configuration_values


app = Flask(__name__)
app.config['SECRET_KEY'] = 'secrete-key'  # change later.

# Creating Flask Global PodCommunicator and connecting to Pod
with app.app_context():
    try:
        pod_communicator = PodCommunicator.get_pod_communicator()
    except Exception as e:
        print("COULD NOT CONNECT TO POD")

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
        configuration = validate_configuration_values(configuration_form)
        if configuration['all_values_valid'] is True:
            pod_communicator.send_configuration()
            return jsonify({'status': 'ok'})
        return jsonify({'error': configuration['error']})
    else:
        return jsonify({'error': configuration_form.errors})


@app.route('/send_command', methods=["POST"])
def send_command():
    command = request.get_json()['command']
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
    with open('LocalStorage/DtsSensors.json') as json_file:
        sensors = json.load(json_file)
    return render_template(
        page+".html",
        active_page=page,
        title=title,
        sensors=sensors,
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
        saved_configuration=LocalStorage.get_default_configuration()
    )




