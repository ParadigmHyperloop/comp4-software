from influxdb import InfluxDBClient
import env_vars


# @TODO cleanup previous code
class SpaceXStatus:
    FAULT = 0
    IDLE = 1
    READY = 2
    PUSHING = 3
    COASTING = 4
    BRAKING = 5


# #TODO double check from newest requirements for SpaceX
class SpaceXPacket:
    def __init__(self, team_id, status=None, position=None, velocity=None,
                 acceleration=None, battery_voltage=None,
                 battery_current=None, battery_temperature=None,
                 pod_temperature=None, stripe_count=None):
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


class SpaceX:
    def __init__(self):
        self.ip = env_vars.spaceX_IP
        self.port = env_vars.spaceX_PORT
        self.team_id = env_vars.team_name

        try:
            # @TODO try connecting
            pass
        except:
            # @TODO if not, error or try again
            pass

    def genSpaceXPacket(self):
        # TODO get pod status and info, create a SpaceXPacket object, return it
        pass

    def send2SpaceX(self):
        spacePacket = self.genSpaceXPacket()
        # @TODO socket.send(spacePacket)


def telem():
    # @TODO heartbeat or connect to pod, do parsing and adding to db
    pass


def main():
    # Create connection to influx database
    influx_db = InfluxDBClient(host=env_vars.influx_host, port=env_vars.influx_port, username=env_vars.influx_user,
                               password=env_vars.influx_pw)
    if env_vars.influx_name not in influx_db.get_list_database():
        influx_db.create_database(env_vars.influx_name)
        influx_db.switch_database(env_vars.influx_name)

    # @TODO spawn threads for pod heartbeat, SpaceX telem


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt as e:
        pass
