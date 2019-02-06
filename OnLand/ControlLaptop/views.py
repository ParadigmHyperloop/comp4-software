from datetime import datetime
# import random
from flask import *
from OnLand.ControlLaptop.config import *

app = Flask(__name__)


@app.route("/")
def index():
    return redirect('/ui/')


@app.errorhandler(404)
def page_not_found(e):
    return render_template('404.html', title="Page Not Found"), 404


@app.route("/_sensorsRefresh")
def sensor_values():
    temp = get_sensor_values()
    return jsonify(tuple(temp))


@app.context_processor
def inject_now():
    return {'now': datetime.utcnow(), 'navigation_bar': NAV_BAR}


@app.route("/ui/", defaults={'path': 'index.html'})
@app.route("/ui/<path:path>")
def ui(path):
    table = generate_sensor_table()

    # idk how this works, came like this, don't wanna break anything
    # generates the side bar
    page = path.split('.')[0]
    if page in NAV_IDS:
        title = NAV_BAR[NAV_IDS.index(page)]['title']
    else:
        title = DEFAULT_TITLE

    return render_template(path, active_page=page, title=title, test=table)
