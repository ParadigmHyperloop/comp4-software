from influxdb import InfluxDBClient
import config
import PodTelem_pb2
import struct

from config import *


# #TODO double check from newest requirements for SpaceX
class SpaceXPacket:
    def __init__(self, team_id, status=None, position=None, velocity=None,
                 acceleration=None, battery_voltage=0,
                 battery_current=0, battery_temperature=0,
                 pod_temperature=0, stripe_count=0):
        self.team_id = team_id
        self.status = status
        self.position = position
        self.velocity = velocity
        self.acceleration = acceleration
        self.battery_voltage = battery_voltage
        self.battery_current = battery_current
        self.battery_temperature = battery_temperature
        self.pod_temperature = pod_temperature
        self.stripe_count = stripe_count
        self.current_sender = None

        def to_bytes(self):
            """Convert to bytes"""
            pattern = '!BB7iI'
            accel = self.acceleration
            if accel >= 1073741823 or accel <= -1073741823:
                accel = 0

            v = self.velocity
            if v >= 1073741823 or v <= -1073741823:
                v = 0

            x = self.position
            if x >= 1073741823 or x <= -1073741823:
                x = 0

            print("{} {} {} {} {} {}".format(pattern, self.team_id, self.status, accel, x, v))
            try:
                b = struct.pack(pattern, self.team_id, self.status, accel,
                                x, v, 0,
                                0, 0,
                                0, 0)
            except Exception as e:
                print(e)
                return b'\0'
            return b


# Includes all SpaceX stuff, connecting to their device, sending data, generating data, wtvr else
class SpaceX:

    global app

    status_map = {
        0: "Fault",
        1: "Safe to approach",
        2: "Ready to launch",
        3: "Launching",
        4: "Coasting",
        5: "Braking",
        6: "Crawling"
    }

    def __init__(self):
        self.ip = config.spaceX_IP
        self.port = config.spaceX_PORT
        self.team_id = config.team_name

        try:
            # @TODO try connecting
            pass
        except:
            # @TODO if not, error or try again
            pass

    def genSpaceXPacket(self):
        # TODO get pod status and info, create a SpaceXPacket object, return it
        packet = SpaceXPacket(team_id=self .team_id)
        # self.socket.sendall(packet)
        pass

    def send2SpaceX(self):
        spacePacket = self.genSpaceXPacket()
        # @TODO self.socket.send(spacePacket)


# Everything telemetry, connecting to pod, parsing incoming telem, adding to db
class Telemetry:
    # @TODO heartbeat or connect to pod, do parsing and adding to db
    pass

    def parseProto(self, input):
        message = PodTelem_pb2.telemetry()
        message.parseFromString(input)
        # @TODO add to db


def main():
    # Set the environment variables here
    global app
    app = DevelopmentConfiguration

    # Create connection to influx database
    influx_db = InfluxDBClient(host=app.influx_host, port=app.influx_port, username=app.influx_user,
                               password=app.influx_pw)
    # add the db if not existent already
    if app.influx_db_name not in influx_db.get_list_database():
        influx_db.create_database(app.influx_db_name)
    influx_db.switch_database(app.influx_db_name)

    # @TODO spawn threads for pod heartbeat, SpaceX telem


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt as e:
        pass
