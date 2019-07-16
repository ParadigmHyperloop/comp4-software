import sys
import socket
import time
import random
from config import POD_COMMANDER_PORT
from Paradigm_pb2 import Telemetry

TEST_MAX_INT32_ = 2147483647
TEST_MAX_UINT32 = 4294967295
TEST_MAX_FLOAT = 1.7976931348623157e+308


def create_telem(packet):
    packet.lp1 = random.randint(1, 101)
    packet.hp = random.randint(1, 101)
    packet.sol1 = random.randint(0, 1)
    packet.pressureVesselTemperature = random.randint(1, 101)
    packet.railTemperature = random.randint(1, 101)
    packet.sol2 = random.randint(0, 1)

    return packet


def create_telemWithFullValues():
    telemetry_packet = Telemetry()

    # States
    telemetry_packet.podState = random.randint(0, 9)
    telemetry_packet.controlsInterfaceState = random.randint(0, 5)
    telemetry_packet.lvdcNodeState = 2
    telemetry_packet.breakNodePerceivedPodState = random.randint(0, 9)
    telemetry_packet.brakeNodeState = random.randint(0, 5)

     # Time
    telemetry_packet.flightTime = TEST_MAX_INT32_

    # telemetry_packet.heartbeatFlags = TEST_MAX_INT32_
    # telemetry_packet.heartbeatValues = TEST_MAX_INT32_

    # navigation
    telemetry_packet.podPosition = TEST_MAX_INT32_ # Is this large enough?
    telemetry_packet.podVelocity = TEST_MAX_INT32_
    telemetry_packet.podAccelerationX = TEST_MAX_INT32_
    telemetry_packet.tachometerVelocity = TEST_MAX_INT32_
    telemetry_packet.resolverVelocity = TEST_MAX_INT32_

    # Inverter
    telemetry_packet.maxIgbtTemperature = TEST_MAX_INT32_
    telemetry_packet.gateDriverTemperature = TEST_MAX_INT32_
    telemetry_packet.inverterControlBoardTemperature = TEST_MAX_INT32_
    telemetry_packet.motorTemperature = TEST_MAX_INT32_
    telemetry_packet.inverterBusVoltage = TEST_MAX_INT32_
    telemetry_packet.inverterHeartbeat = TEST_MAX_INT32_
    telemetry_packet.motorSpeed = TEST_MAX_INT32_
    telemetry_packet.inverterFaultBitLo = TEST_MAX_INT32_
    telemetry_packet.inverterFaultBitHi = TEST_MAX_INT32_

    # HV-BMS
    telemetry_packet.hvBatteryPackVoltage = TEST_MAX_FLOAT
    telemetry_packet.hvBatteryPackCurrent = TEST_MAX_FLOAT
    telemetry_packet.hvBatteryPackMaxCellTemperature = TEST_MAX_FLOAT
    telemetry_packet.hvBatteryPackMaxCellVoltage = TEST_MAX_FLOAT
    telemetry_packet.hvBatteryPackMinimumCellVoltage = TEST_MAX_FLOAT
    telemetry_packet.hvBatteryPackStateOfCharge = TEST_MAX_INT32_

    # LV-BMS
    telemetry_packet.lv1BatteryPackStateOfCharge = TEST_MAX_INT32_
    telemetry_packet.lv1BatteryPackVoltage = TEST_MAX_FLOAT
    telemetry_packet.lv1BatteryPackCellTemperature = TEST_MAX_FLOAT
    telemetry_packet.lv2BatteryPackStateOfCharge = TEST_MAX_INT32_
    telemetry_packet.lv2BatteryPackVoltage = TEST_MAX_FLOAT
    telemetry_packet.lv2BatteryPackCellTemperature = TEST_MAX_FLOAT
    telemetry_packet.hvFaultCode1 = TEST_MAX_INT32_
    telemetry_packet.hvFaultCode2 = TEST_MAX_INT32_

    # Brake Node
    telemetry_packet.solenoid1 = False
    telemetry_packet.solenoid2 = False
    telemetry_packet.solenoid3 = False
    telemetry_packet.solenoid4 = False
    telemetry_packet.highPressure = TEST_MAX_FLOAT
    telemetry_packet.lowPressure1 = TEST_MAX_FLOAT
    telemetry_packet.lowPressure2 = TEST_MAX_FLOAT
    telemetry_packet.lowPressure3 = TEST_MAX_FLOAT
    telemetry_packet.lowPressure4 = TEST_MAX_FLOAT
    telemetry_packet.pressureVesselTemperature = TEST_MAX_FLOAT
    telemetry_packet.coolantTemperature = TEST_MAX_FLOAT

    telemetry_packet.enclosurePressure = TEST_MAX_FLOAT
    telemetry_packet.enclosureTemperature = TEST_MAX_FLOAT

    # Rear Node
    telemetry_packet.tubePressure = TEST_MAX_FLOAT
    telemetry_packet.coolantPressure1 = TEST_MAX_FLOAT
    telemetry_packet.coolantPressure2 = TEST_MAX_FLOAT


# Ghost Train
    telemetry_packet.pack1Voltage = TEST_MAX_INT32_
    telemetry_packet.pack2Voltage = TEST_MAX_INT32_
    telemetry_packet.pack1Current = TEST_MAX_INT32_
    telemetry_packet.pack2Current = TEST_MAX_INT32_
    telemetry_packet.rail1Voltage = TEST_MAX_INT32_
    telemetry_packet.rail2Voltage = TEST_MAX_INT32_
    telemetry_packet.rail3Voltage = TEST_MAX_INT32_
    telemetry_packet.rail4Voltage = TEST_MAX_INT32_
    telemetry_packet.railVoltageFlag = TEST_MAX_INT32_

    telemetry_packet.railCurrentSensor1 = TEST_MAX_INT32_
    telemetry_packet.railCurrentSensor2 = TEST_MAX_INT32_
    telemetry_packet.railCurrentSensor3 = TEST_MAX_INT32_
    telemetry_packet.railCurrentSensor4 = TEST_MAX_INT32_
    telemetry_packet.railCurrentSensor5 = TEST_MAX_INT32_
    telemetry_packet.railCurrentSensor6 = TEST_MAX_INT32_
    telemetry_packet.railCurrentSensor7 = TEST_MAX_INT32_
    telemetry_packet.railCurrentSensor8 = TEST_MAX_INT32_
    telemetry_packet.railCurrentFlag = TEST_MAX_INT32_

    telemetry_packet.motorTorque = TEST_MAX_UINT32
    telemetry_packet.flightDistance = TEST_MAX_FLOAT
    telemetry_packet.maxFlightTime = TEST_MAX_UINT32
    telemetry_packet.startTorque = TEST_MAX_UINT32
    telemetry_packet.accelerationTime = TEST_MAX_UINT32
    telemetry_packet.taxi = False
    telemetry_packet.expectedTubePressure = TEST_MAX_UINT32

    telemetry_packet.tachRpm = TEST_MAX_UINT32
    telemetry_packet.irRpm = TEST_MAX_UINT32
    telemetry_packet.tachDistance = TEST_MAX_FLOAT
    telemetry_packet.irDistance = TEST_MAX_FLOAT

    return telemetry_packet


def main():
    sudp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        pod_data = create_telemWithFullValues()
    except Exception as e:
        print(e)

    msgBodySizet = sys.getsizeof(pod_data.SerializeToString())

    print(f'Size of max: {msgBodySizet}')

    stcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    stcp.bind(("127.0.0.1", POD_COMMANDER_PORT))
    stcp.listen()
    conn, addr = stcp.accept()
    print(conn, addr)

    pod_data = Telemetry()

    while(1):
        sudp.sendto(pod_data.SerializeToString(), ("127.0.0.1", 4000))

        data = conn.recv(1024)

        if data:
            print("recved:", data)
            conn.send(b'1')

        time.sleep(1)

    stcp.close()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('Killed by user')
        exit(0)
