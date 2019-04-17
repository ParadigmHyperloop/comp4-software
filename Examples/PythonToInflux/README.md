# PythonToInflux

Before running you will need to configure the path of the InfluxDB in the
influxd.conf file with 'dir' and 'wal-dir'. Currently these paths are set to my
computer and I am unsure if they will automatically override if the path doesn't
exist on your computer.

To test the program. Launch InfluxDB in one shell with 'influxd', in a separate shell
run 'python3 main.py' and follow the prompts.

The program will input data into the database with each press of the enter key.

See Git branch SOF-102 for other examples of Database logging with Influx.
