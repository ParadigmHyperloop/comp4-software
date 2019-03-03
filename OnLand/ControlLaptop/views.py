from datetime import datetime
# import random
from flask import *
from config import *
from forms import FlightConfigurationForm, validate_configuration_values

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secrete-key' # change later


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
            # todo: send to POD and return these new values.
            return jsonify({'status': 'ok'})
        return jsonify({'error': configuration['error']})
    else:
        return jsonify({'error': configuration_form.errors})


@app.route("/ui/", defaults={'path': 'index.html'})
@app.route("/ui/<path:path>")
def ui(path):
    table = generate_sensor_table()

    # Checking to return Page Title
    configureation_fomr = None
    page = path.split('.')[0]
    if page in NAV_IDS:
        title = NAV_BAR[NAV_IDS.index(page)]['title']
    else:
        title = DEFAULT_TITLE
    if path == 'profile':
        # TODO:
        #   - Read from JSON FILE if it exists
        #   - and set default Form values.
        print('Setting up configuration form and validation')

    return render_template(
        path,
        active_page=page,
        title=title,
        sensors=[TempSensor()],
        configuration_form=(FlightConfigurationForm() if page == 'profile' else None)
    )


# TODO:
#   This sensor list should be read and generated from global/JSON file.
class TempSensor:
    def __init__(self):
        self.name = 'name'
        self.min = 0
        self.max = 100
