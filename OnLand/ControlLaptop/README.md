# HyperLoop Pod Terminal 

used to communicate with Pod

ported over from [Paradigm Hyperloop/Web-UI](https://github.com/ParadigmHyperloop/web-UI)

# License
Copyright 2017 Creative Tim (http://www.creative-tim.com)
Licensed under MIT (https://github.com/creativetimofficial/light-bootstrap-dashboard/blob/master/LICENSE.md)


- run App (if using terminal):
  - `python pod_echo_server` - To Connect to 
  - `export FLASK_APP=controlLaptop.py`
  - `flask run` 


#### Environment (WIP):
- Pycharm: 
    1) Open Project with ControlLaptop as the root folder.
    2) Navigate to: Preferences --> Project Interpreter.
    3) click add and select existing environment.
    4) Select python labeled: `controlLaptop`


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
- Figure out Protobuf packet content to be streamed.
- Change all pressure units to a multiple of Pascals (e.g. kPa for environment, MPa for pressure tanks).
- Make buttons grey out/inactive when pod isnt in the correct state.
    - "Fire" command should be password protected

### Documentation
Description of file content and operation

- `app.py` contains Python array for the sidebar generation
- `base.html` contains `_header.html`, `_sidebar.html` and `_toolbar.html`
- `index.html` extends `base.html`, and adds in cards from `_cards` (tables)
- sensor table is now dynamically generated from database content, as well as update in real time.

- data in the different sensor cards (the first row of sensors) are all static atm, once sensors are defined can have them
update in real time
