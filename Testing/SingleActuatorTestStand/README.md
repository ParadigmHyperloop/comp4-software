# Single Actuator Test Stand (SATS) Control and Analysis Software

This software suite interprets, displays and logs data from the SATS. This data is received from sensors interfacing with an Arduino Uno. The Uno then communicates this data over a serial connection to a control laptop where it is read. The software also allows for full control over the stand's solenoid-activated brakes, using the same serial connection.

![image](https://user-images.githubusercontent.com/44215543/53060162-0c704800-3494-11e9-808b-b9991d69bbf5.png)
_Screenshot of the Grafana dashboard (outdated)_

## Installation and Dependencies

1. Install InfluxDB. Instructions are available for Mac and Linux from the [InfluxDB installation page](https://docs.influxdata.com/influxdb/v1.7/introduction/installation/)
2. Install Grafana. Instructions are available for Mac, Windows and Linux from the [Grafana installation page](http://docs.grafana.org/installation/)
3. Upload [test_stand.ino](./Arduino/test_stand/test_stand.ino) to an Arduino Uno
4. Install [Python](https://www.python.org/downloads/) 3.6 or later
5. Create a virtual environment with:

    ```
    python -m venv ENV_NAME
    ```
    or if you have [virtualenvwrapper](https://virtualenvwrapper.readthedocs.io/en/latest/):

    ```
    mkvirtualenv ENV_NAME
    ```
6. Activate your virtual environment
7. Install Python dependencies by navigating to the directory with `requirements.txt` and running:
    ```
    pip install -r requirements.txt
    ```

## Usage

1. Connect all sensors to the Arduino Uno and connect the Uno to the Control Laptop via USB
2. Start the Grafana server. Information on how to do this can be found at the [installation page](http://docs.grafana.org/installation/)
3. Upload the Grafana dashboard:
   - Visit your Grafana homepage (default address is `localhost:30000`).
   - On the left-hand sidebar, select the `+` icon, then `Import`.
   - Select `Upload .json file` and upload [test_stand_dasboard.json](./Grafana/test_stand_dashboard.json).
   - Change the dashboard's `uid` in the next screen, select an InfluxDB data source, then press `import`.
4. Configure the dashboard:
   - Select the cog on the left-hand sidebar, navigate to `Data Sources`, `InfluxDB` then `InfluxDB Details`.
   - Change the `Database` field to the name of the database you wish to use.
5. Start the InfluxDB server. Information on how to do this can be found at the [installation page](https://docs.influxdata.com/influxdb/v1.7/introduction/installation/)
6. Create an environment variable called `TEST_STAND_DB` with the name of the InfluxDB database you'll be using to log data:

    ```
    export TEST_STAND_DB=DATABASE_NAME
    ```
    You don't need to create this database - just provide the name you'd like to use

7. Activate your Python virtual environment
8. Run the script:
   - Navigate to the `ControlLaptop` directory and run:
     ```
     python test_stand.py
     ```
   - Select a the serial port the Arduino is connected to from the list that appears
   - When ready, type `start` to start the test
   - Control the solenoid using the enter/return key
   - When finished, type `q` and press enter/return
   - Make any notes you'd like about the test, then press enter/return twice

After any test, inside the folder of the Python  script, there should be a `TestData` directory. Inside this directory there should be a separate directory for each test, each with a `.txt` file (for notes) and a `.csv` file (for data). A new test can then be started by running `test_stand.py` again

## Files

| File | Purpose |
|:----:|:-------:|
| ControlLaptop/test_stand.py | __Main Control Laptop file__ - handles user interaction and threading for the serial port listener|
| ControlLaptop/database.py | Logs data to and pulls data from the InfluxDB database |
| ControlLaptop/serial_manager.py | Reads from and writes to the Arduino serial port |
| ControlLaptop/csv_logger.py | Logs test data to a `.csv` file and writes notes to a `.txt` file once the test is finished. |
| Arduino/test_stand/test_stand.ino | Arduino File - Reads all sensors data and sends that data over serial while listening for solenoid commands from the Control Laptop |
| Grafana/test_stand_dashboard.json | A template for the Grafana dashboard used to view the live data |
