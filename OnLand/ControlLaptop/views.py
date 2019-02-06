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
    return {'now': datetime.utcnow(),
            'navigation_bar': NAV_BAR}


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


# # DEPRECATED
# @app.route("/_refreshTables")
# def DEPRECATED_get_sensor_values():
#     # generates random dummy values to be returned
#     status = ['0', '1']
#     values = []
#     for x in range(0,100):
#         values.append(x)
#
#     return jsonify(value00=random.choice(values), value01=random.choice(values), value02=random.choice(values), status0=random.choice(status),
#                    value10=random.choice(values), value11=random.choice(values), value12=random.choice(values), status1=random.choice(status),
#                    value20=random.choice(values), value21=random.choice(values), value22=random.choice(values), status2=random.choice(status),
#                    value30=random.choice(values), value31=random.choice(values), value32=random.choice(values), status3=random.choice(status),
#                    )
