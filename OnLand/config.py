# Influx Data Storage

influx_host = "localhost"
influx_port = 8086
influx_name = "paradigm_hyperloop"
influx_user = "paradigm"
influx_pw = "hyperloop"

LOCAL_HOST = '127.0.0.1'

# Space X Packet
spaceX_IP = "something"
spaceX_PORT = "something"

team_name = "ParaHyp"
MAX_MESSAGE_SIZE = 1024
UDP_RECV_BUFFER_SIZE = 50


# Udp Telemetry Stream
UDP_TELEM_TIMEOUT = 3  # How long to wait without receiving telemetry before deciding connection is lost
UDP_TELEM_PORT = 7000  # Control Server Port to receive telemetry over
TELEMETRY_BROADCAST_FREQUENCY = 500  # Minimum time between telemetry broadcasts


# SocketIO
SOCKET_SERVER = 'http://localhost:5000'


# Pod
# POD_IP = '127.0.0.1'
POD_IP = '192.168.0.10'
#POD_IP = '192.168.7.2'
POD_CONFIG_PORT = 3001


# Heartbeat & Commander
POD_COMMANDER_PORT = 6000
COMMANDER_PULSE_SPEED = 1000  # Time to wait before sending ping to pod
COMMANDER_BACKUP_PULSE = 1500  # Time to wait without hearing back before sending another ping to the pod
COMMANDER_TIMEOUT_TIME = 4000  # How long do we wait to hear back from the pod before we decided its lost
COMMANDER_BROADCAST_FREQUENCY = 4000  # How often it alerts the front end the connection is still good.




