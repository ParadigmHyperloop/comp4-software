# Single Actuator Test Stand (SATS) Control and Analysis Software
___
This software suite interprets, displays and logs data from the SATS. This data is received from sensors interfacing with an Arduino Uno. The Uno then communicates this data over a serial connection to a control laptop where it is read. The software also allows for full control over the stand's solenoid-activated brakes, using the same serial connection.

## Installation and Dependencies

1. Install InfluxDB. Instructions are available for Mac and Linux from the [InfluxDB installation page](https://docs.influxdata.com/influxdb/v1.7/introduction/installation/)
- Install Grafana. Instructions are available for Mac, Windows and Linux from the [Grafana installation page](http://docs.grafana.org/installation/)
- Upload [test_stand.ino](./Arduino/test_stand.ino) to an Arduino Uno
- Install [Python](https://www.python.org/downloads/) 3.6 or later
- Create a virtual environment with:
    ```
    python -m venv ENV_NAME
    ```
    or if you have [virtualenvwrapper](https://virtualenvwrapper.readthedocs.io/en/latest/):
    ```
    mkvirtualenv ENV_NAME
    ```
- Activate your virtual environment
- Install Python dependencies by navigating to the directory with `requirements.txt` and running:
    ```
    pip install -r requirements.txt
    ```

## Usage

1. Connect all sensors to the Arduino Uno and connect the Uno to the Control Laptop via USB
- Start the Grafana server. Information on how to do this can be found at the [installation page](http://docs.grafana.org/installation/)
- Open a [snapshot of the Grafana dashboard](https://snapshot.raintank.io/dashboard/snapshot/vAmTL0ja5RnmioAxvtjzRIUPH7ivOTeL) for live data viewing
- Start the InfluxDB server. Information on how to do this can be found at the [installation page](https://docs.influxdata.com/influxdb/v1.7/introduction/installation/)
- Create an environment variable called `TEST_STAND_DB` with the name of the InfluxDB you'll be using to log the data with:
```
export TEST_STAND_DB=DATABASE_NAME
```
You don't need to create this database - just provide the name you'd like to use
- Activate your Python virtual environment
- Run the script:
 - Navigate to the `ControlLaptop` directory and run:
```
python test_stand.py
```
 - Select a the serial port the Arduino is connected to from the list that appears
 - When ready, type `start` to start the test
 - Control the solenoid using the enter/return key
 - When finished, type `q` and press enter/return
 - Make any notes you'd like about the test, then press enter/return twice

After any test, inside the folder of the Python  script, there should be a `TestData` directory. Inside this directory there should be a separate directory for each test, with a `.txt` file (for notes) and a `.csv` file (for data). A new test can then be started by running `test_stand.py` again

## Code

| File | Purpose |
|:----:|:-------:|
| test_stand.ino | Main Arduino file - read all sensors and send that data over serial while listening for solenoid commands form the Control Laptop |
|test_stand.py||
|database.py||
|serial_manager.py||
|csv_logger.py||
