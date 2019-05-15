import socketio
import random
import json


sio = socketio.Client()

@sio.on('connect')
def on_connect():
    print('Connected to Server')
    sio.emit('connected', 'test Script')

def getTestPacket():
    packet = dict()
    packet['podStat'] = random.randint(0, 2)
    packet['lp1'] = random.randint(1, 101)
    packet['hp'] = random.randint(1, 101)
    packet['sol1'] = random.randint(0, 1)
    packet['pressureVesselTemperature'] = random.randint(1, 101)
    packet['railTemperature'] = random.randint(1, 101)
    packet['sol2'] = random.randint(0, 1)
    return json.dumps(packet)


def main():
    
    sio.connect('http://localhost:5000')
    sio.emit('log_telemetry', getTestPacket())


if __name__ == '__main__':
    main()

