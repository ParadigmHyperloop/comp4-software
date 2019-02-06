# Hyperloop Pod Terminal 

used to communicate with Pod

ported over from [Paradigm Hyperloop/Web-UI](https://github.com/ParadigmHyperloop/web-UI)

### How to:
- Dependencies
    - Linux
        -  Arch based: `sudo pacman -S influxdb protobuf`
        - Debian based: `sudo apt-get install influxdb protobuf`
    - MacOS (assuming you have brew installed)
        - `brew install influxdb protobuf`
    
    To make sure it's all there, try
    -  `protoc --version`
    -  `influx --version`

- clone or download the repo/branch
- open it in your preferred IDE (I recommend PyCharm)
    - If using PyCharm, it should warn you that no Python interpreter has been defined, clicking on the message will 
    land you on a window allowing you to create a new venv, make sure to uncheck `inherit global site package` and 
    create the new venv in `/OnLand/ControlLaptop/venv`
    - If not using PyCharm, or if it doesn't do it automatically, do the following in a Terminal shell, or in the Terminal
    from PyCharm (bottom bar usually): 
        - `cd` into the git directory (`cd ./comp4-software/OnLand/ControlLaptop`)
        - create new venv (`python -m venv ./venv --no-site-packages`) 
        - activate it (`source ./bin/activate`
    - install pip dependencies (`sudo pip install -r /path/to/requirements.txt`)
- run `comp4-software/Examples/WebApp/dbExampleWorking.py` to initialize db with dummy data
- run `app.py`
- navigate to `localhost:5000`


### Troubleshooting
##### Errors related to InfluxDB
- make sure Influx service is running (on Linux, `systemctl status influxdb`)
    - If not, start it: `sudo systemctl start influxdb`
    - OPTIONAL: make it start on boot time: `sudo systemctl enable influxdb`

##### Error with packages (influxdb and/or protobuf)
- Installation errors
    - If the error “Calling needs :cxx14 is disabled! There is no replacement.” appears. 
    run the command `brew cleanup`, or `brew tap ——` the reported directory.

### What is left:
- cleanup the HTML and CSS: mostly done
- implement TCP server and telemetry (sending commands, fetching telemetry from node boards and wtvr else)
- implement Google Protobuff: examples in `Examples` folder
- Improve project structure (separate `@routes` and scripts from `app.py`)


### Documentation
Description of file content and operation

- `app.py` contains Python array for the sidebar generation
- `base.html` contains `_header.html`, `_sidebar.html` and `_toolbar.html`
- `index.html` extends `base.html`, and adds in cards from `_cards` (tables)
- sensor table is now dynamically generated from database content, as well as update in real time.


### TODO
- keep track of pod state: 
    - ~proposed system: have in `app.py` a Pod Object or similar that keeps track of current state~
    - ~or, have that information stored in InfluxDB alongside telemetry data, and whenever info is needed 
    either JS or Py can ping the db:~ Done. Needs to figure out what exact data is going to be streamed.
- Change all pressure units to a multiple of Pascals (e.g. kPa for environment, MPa for pressure tanks).
- Make buttons grey out/inactive when pod isnt in the correct state.
    - "Fire" command should be password protected