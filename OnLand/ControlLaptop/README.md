# Hyperloop Pod Terminal 

used to communicate with Pod

ported over from [Paradigm Hyperloop/Web-UI](https://github.com/ParadigmHyperloop/web-UI)

### How to:
- clone or download
- open it in your preferred IDE (I recommend PyCharm)
- have a InfluxDB instance running, in my case db is called `test` and run `test1` in `dbExampleWorking.py` with different names.
- start `app.py`
- navigate to `localhost:5000`


### What is left:
- go through the rest of the tables to put IDs on values that need to be accessed and changed by JS
- cleanup the HTML and CSS
- implement the Python functions to communicate with Pod (opening TCP/UDP ports, sending commands, fetching telemetry)
- implement Google Protobuff



### Documentation
Description of file content and operation

- `app.py` contains Python array for the sidebar generation
- `base.html` contains `_header.html`, `_sidebar.html` and `_toolbar.html`<sup>1</sup>
- `index.html` extends `base.html`, and adds in cards from `_cards` (tables)
- sensor table is now dynamically generated from database content, as well as update in real time.


### TODO
- keep track of pod state: 
    - ~proposed system: have in `app.py` a Pod Object or similar that keeps track of current state~
    - ~or, have that information stored in InfluxDB alongside telemetry data, and whenever info is needed either JS or Py can ping the db:~ Done. Needs to figure out what exact data is going to be streamed.
