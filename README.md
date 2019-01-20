# Hyperloop Pod Terminal 

used to communicate with Pod

ported over from [Paradigm Hyperloop/Web-UI](https://github.com/ParadigmHyperloop/web-UI)

### How to:
- make sure you have InfluxDB and Grafana installed (on Arch, sudo pacman -S influxdb grafana), 
and optionally install Chronograf too (available from AUR for Arch) 
- clone or download this repo
- open it in your preferred IDE (I recommend PyCharm)
- install the requirements from pip (`pip install -r requirements.txt`)
- run `dbExampleWorking.py` to initialize db with dummy data
    - if you have any issues, check out `Troubleshooting` down below
- start `app.py`
- navigate to `localhost:5000`

### What is left:
- cleanup the HTML and CSS: somewhat done, find the equivalent of width stuff in `demo.css` in the Bootstrap libraries
- implement the Python functions to communicate with Pod (opening TCP/UDP ports, sending commands, fetching telemetry)
- implement Google Protobuf
- ~go through the rest of the tables to put IDs on values that need to be accessed and changed by JS~ @DONE
- ~fix some exceptions raised by Flask while rendering (probably non-existent files)~ @DONE



### Documentation
Description of file content and operation
- `app.py` contains Python array for the sidebar generation, as well as dynamic table generation for sensor data, 
and on the long run will also have all the pod communication stuff
- `base.html` contains `_header.html`, `_sidebar.html` and `_toolbar.html`
- `index.html` extends `base.html`, and adds in cards from `_cards` (tables)


### TODO
- keep track of pod state: 
    - ~proposed system: have in `app.py` a Pod Object or similar that keeps track of current state~ kinda bs idea
    - or, have that information stored in InfluxDB alongside telemetry data, and whenever info is needed  Py can ping 
    the db: easiest and most sensible imo


### Troubleshooting
- If you have issues with InfluxDB not starting, make sure to transfer ownership of `/var/lib/influxdb` to 
influxdb:influxdb (sudo chown influxdb:influxdb /var/lib/influxdb)