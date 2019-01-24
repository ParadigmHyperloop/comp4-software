# Hyperloop Pod Terminal 

used to communicate with Pod

ported over from [Paradigm Hyperloop/Web-UI](https://github.com/ParadigmHyperloop/web-UI)

### How to:
- install InfluxDB (on Arch based: `sudo pacman -S influxdb`, on Debian based: `sudo apt-get install influxdb`)
- enable and start InfluxDB service (`sudo systemctl enable influxdb && sudo systemctl start influxdb`)
- clone or download
- open it in your preferred IDE (I recommend PyCharm)
    - If using PyCharm, it should automatically ask which compiler you want to use, where you can chose to create 
    a new venv)
    - If not using PyCharm, or if it doesn't do it automatically, do the following in a Terminal shell, or in the Terminal
    from PyCharm (bottom bar usually): 
        - `cd` into the git directory (`cd /comp4-software/OnLand/ControlLaptop`)
        - create new venv (`python -m venv ./venv`) 
        - activate it (`source ./bin/activate`
        - install pip dependencies (`sudo pip install -r /path/to/requirements.txt`)
- run `comp4-software/Examples/WebApp/dbExampleWorking.py` to initialize db with dummy data
- run `app.py`
- navigate to `localhost:5000`


### What is left:
- ~go through the rest of the tables to put IDs on values that need to be accessed and changed by JS~ Done automatically
- cleanup the HTML and CSS: mostly done
- implement the Python functions to communicate with Pod (opening TCP/UDP ports, sending commands, fetching telemetry): 
examples in Examples folder
- implement Google Protobuff


### Documentation
Description of file content and operation

- `app.py` contains Python array for the sidebar generation
- `base.html` contains `_header.html`, `_sidebar.html` and `_toolbar.html`
- `index.html` extends `base.html`, and adds in cards from `_cards` (tables)
- sensor table is now dynamically generated from database content, as well as update in real time.


### TODO
- keep track of pod state: 
    - ~proposed system: have in `app.py` a Pod Object or similar that keeps track of current state~
    - ~or, have that information stored in InfluxDB alongside telemetry data, and whenever info is needed either JS or Py can ping the db:~ Done. Needs to figure out what exact data is going to be streamed.