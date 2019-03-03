class DevelopmentConfiguration:
    influx_host = "localhost"
    influx_port = 8086
    influx_db_name = "dev"
    influx_user = "paradigm"
    influx_pw = "hyperloop"


class TestingConfiguration:
    influx_host = "localhost"
    influx_port = 8086
    influx_db_name = "test"
    influx_user = "paradigm"
    influx_pw = "hyperloop"

    team_name = "ParaHyp"


class FlightConfiguration:
    influx_host = "localhost"
    influx_port = 8086
    influx_db_name = "CompIV"
    influx_user = "paradigm"
    influx_pw = "hyperloop"

    spaceX_IP = "something"
    spaceX_PORT = "something"
