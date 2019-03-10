from datetime import datetime
# import random
from flask import *

from LocalStorage.ConfigurationSotrage import LocalStorage
from SocketController import PodCommunicator
from config import *
from forms import FlightConfigurationForm, validate_configuration_values


app = Flask(__name__)
app.config['SECRET_KEY'] = 'secrete-key' # change later

with app.app_context():
    try:
        pod_communicator = PodCommunicator.get_pod_communicator()
    except:
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


@app.route("/ui/", defaults={'path': 'index.html'})
@app.route("/ui/<path:path>")
def ui(path):
    # Checking to return Page Title
    page = path.split('.')[0]
    if page in NAV_IDS:
        title = NAV_BAR[NAV_IDS.index(page)]['title']
    else:
        title = DEFAULT_TITLE
    if page == 'profile':
        return get_flight_profile_template(path, page, title)
    else:
        return render_template(
            path,
            active_page=page,
            title=title,
            sensors=[LocalStorage.get_sensors()],
        )


def get_flight_profile_template(path, page, title):
    return render_template(
        path,
        active_page=page,
        title=title,
        configuration_form=FlightConfigurationForm(),
        saved_configuration=LocalStorage.get_default_configuration()
    )
