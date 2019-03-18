# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: PodTelem.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


import States_pb2 as States__pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='PodTelem.proto',
  package='pds',
  syntax='proto2',
  serialized_options=None,
  serialized_pb=_b('\n\x0ePodTelem.proto\x12\x03pds\x1a\x0cStates.proto\"\x8c\x0f\n\ttelemetry\x12\x1c\n\x08podState\x18\x01 \x01(\x0e\x32\n.PodStates\x12(\n\x0e\x62reakNodeState\x18\x02 \x01(\x0e\x32\x10.BrakeNodeStates\x12 \n\nmotorState\x18\x03 \x01(\x0e\x32\x0c.MotorStates\x12\x38\n\x16\x63ontrolsInterfaceState\x18\x04 \x01(\x0e\x32\x18.ControlsInterfaceStates\x12&\n\rinverterState\x18\x05 \x01(\x0e\x32\x0f.InverterStates\x12&\n\rlvdcNodeState\x18\x36 \x01(\x0e\x32\x0f.LvdcNodeStates\x12\x38\n\x16\x62\x61tteryManagementState\x18\x19 \x01(\x0e\x32\x18.BatteryManagementStates\x12.\n\x1a\x62reakNodePerceivedPodState\x18\x1f \x01(\x0e\x32\n.PodStates\x12&\n\rrearNodeState\x18/ \x01(\x0e\x32\x0f.RearNodeStates\x12\x13\n\x0bmissionTime\x18\x06 \x01(\x05\x12\x12\n\nflightTime\x18\x07 \x01(\x05\x12\x16\n\x0eheartbeatFlags\x18\x08 \x03(\x05\x12\x17\n\x0fheartbeatValues\x18G \x03(\x05\x12\x13\n\x0bpodPosition\x18\t \x01(\x05\x12\x13\n\x0bpodVelocity\x18\n \x01(\x05\x12\x18\n\x10podAccelerationX\x18\x0b \x01(\x05\x12\x18\n\x10podAccelerationY\x18\x0c \x01(\x05\x12\x18\n\x10podAccelerationZ\x18\r \x01(\x05\x12\x13\n\x0bimuVelocity\x18\x0e \x01(\x05\x12\x18\n\x10imuAccelerationX\x18\x0f \x01(\x05\x12\x1a\n\x12tachometerVelocity\x18\x10 \x01(\x05\x12\x18\n\x10resolverVelocity\x18\x11 \x01(\x05\x12\x1c\n\x14inverterTemperature1\x18\x12 \x01(\x05\x12\x1c\n\x14inverterTemperature2\x18\x13 \x01(\x05\x12\x1c\n\x14inverterTemperature3\x18\x14 \x01(\x05\x12\x14\n\x0cmotorCurrent\x18\x15 \x01(\x05\x12\x14\n\x0cmotorVoltage\x18\x16 \x01(\x05\x12\x18\n\x10inverterVoltages\x18\x17 \x01(\x05\x12\x18\n\x10inverterCurrents\x18\x18 \x01(\x05\x12\x1b\n\x13\x62\x61tteryPackVoltages\x18\x1a \x03(\x05\x12\x1a\n\x12\x62\x61tteryPackCurrent\x18\x1b \x03(\x05\x12\x1f\n\x17\x62\x61tteryCellTemperatures\x18\x1c \x03(\x05\x12!\n\x19\x62\x61tteryPackMinimumVoltage\x18\x1d \x01(\x05\x12\x1d\n\x15\x62\x61tteryPackMaxVoltage\x18\x1e \x01(\x05\x12\x0c\n\x04sol1\x18  \x01(\x08\x12\x0c\n\x04sol2\x18! \x01(\x08\x12\x0c\n\x04sol3\x18\" \x01(\x08\x12\x0c\n\x04sol4\x18# \x01(\x08\x12\x0c\n\x04sol5\x18$ \x01(\x08\x12\x0c\n\x04sol6\x18% \x01(\x08\x12\n\n\x02hp\x18& \x01(\x05\x12\x0b\n\x03lp1\x18\' \x01(\x05\x12\x0b\n\x03lp2\x18( \x01(\x05\x12\x0b\n\x03lp3\x18) \x01(\x05\x12\x0b\n\x03lp4\x18* \x01(\x05\x12!\n\x19pressureVesselTemperature\x18+ \x01(\x05\x12$\n\x1c\x65lectronicsEnclosurePressure\x18, \x01(\x05\x12\x19\n\x11railDisplacementX\x18- \x01(\x05\x12\x19\n\x11railDisplacementY\x18. \x01(\x05\x12\x14\n\x0ctubePressure\x18\x30 \x01(\x05\x12\x18\n\x10\x63oolantPressure1\x18\x31 \x01(\x05\x12\x18\n\x10\x63oolantPressure2\x18\x32 \x01(\x05\x12\x18\n\x10\x63oolantPressure3\x18\x33 \x01(\x05\x12\x1a\n\x12\x63oolantTemperature\x18\x34 \x01(\x05\x12\x17\n\x0frailTemperature\x18\x35 \x01(\x05\x12\x14\n\x0cpack1Voltage\x18\x37 \x01(\x05\x12\x14\n\x0cpack2Voltage\x18\x38 \x01(\x05\x12\x14\n\x0cpack1Current\x18\x39 \x01(\x05\x12\x14\n\x0cpack2Current\x18: \x01(\x05\x12\x14\n\x0crail1Voltage\x18; \x01(\x05\x12\x14\n\x0crail2Voltage\x18< \x01(\x05\x12\x14\n\x0crail3Voltage\x18= \x01(\x05\x12\x14\n\x0crail4Voltage\x18> \x01(\x05\x12\x17\n\x0frailVoltageFlag\x18H \x01(\x05\x12\x1a\n\x12railCurrentSensor1\x18? \x01(\x05\x12\x1a\n\x12railCurrentSensor2\x18@ \x01(\x05\x12\x1a\n\x12railCurrentSensor3\x18\x41 \x01(\x05\x12\x1a\n\x12railCurrentSensor4\x18\x42 \x01(\x05\x12\x1a\n\x12railCurrentSensor5\x18\x43 \x01(\x05\x12\x1a\n\x12railCurrentSensor6\x18\x44 \x01(\x05\x12\x1a\n\x12railCurrentSensor7\x18\x45 \x01(\x05\x12\x1a\n\x12railCurrentSensor8\x18\x46 \x01(\x05\x12\x17\n\x0frailCurrentFlag\x18I \x01(\x05')
  ,
  dependencies=[States__pb2.DESCRIPTOR,])




_TELEMETRY = _descriptor.Descriptor(
  name='telemetry',
  full_name='pds.telemetry',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='podState', full_name='pds.telemetry.podState', index=0,
      number=1, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='breakNodeState', full_name='pds.telemetry.breakNodeState', index=1,
      number=2, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='motorState', full_name='pds.telemetry.motorState', index=2,
      number=3, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='controlsInterfaceState', full_name='pds.telemetry.controlsInterfaceState', index=3,
      number=4, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='inverterState', full_name='pds.telemetry.inverterState', index=4,
      number=5, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='lvdcNodeState', full_name='pds.telemetry.lvdcNodeState', index=5,
      number=54, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='batteryManagementState', full_name='pds.telemetry.batteryManagementState', index=6,
      number=25, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='breakNodePerceivedPodState', full_name='pds.telemetry.breakNodePerceivedPodState', index=7,
      number=31, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='rearNodeState', full_name='pds.telemetry.rearNodeState', index=8,
      number=47, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='missionTime', full_name='pds.telemetry.missionTime', index=9,
      number=6, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='flightTime', full_name='pds.telemetry.flightTime', index=10,
      number=7, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='heartbeatFlags', full_name='pds.telemetry.heartbeatFlags', index=11,
      number=8, type=5, cpp_type=1, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='heartbeatValues', full_name='pds.telemetry.heartbeatValues', index=12,
      number=71, type=5, cpp_type=1, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='podPosition', full_name='pds.telemetry.podPosition', index=13,
      number=9, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='podVelocity', full_name='pds.telemetry.podVelocity', index=14,
      number=10, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='podAccelerationX', full_name='pds.telemetry.podAccelerationX', index=15,
      number=11, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='podAccelerationY', full_name='pds.telemetry.podAccelerationY', index=16,
      number=12, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='podAccelerationZ', full_name='pds.telemetry.podAccelerationZ', index=17,
      number=13, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='imuVelocity', full_name='pds.telemetry.imuVelocity', index=18,
      number=14, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='imuAccelerationX', full_name='pds.telemetry.imuAccelerationX', index=19,
      number=15, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='tachometerVelocity', full_name='pds.telemetry.tachometerVelocity', index=20,
      number=16, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='resolverVelocity', full_name='pds.telemetry.resolverVelocity', index=21,
      number=17, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='inverterTemperature1', full_name='pds.telemetry.inverterTemperature1', index=22,
      number=18, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='inverterTemperature2', full_name='pds.telemetry.inverterTemperature2', index=23,
      number=19, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='inverterTemperature3', full_name='pds.telemetry.inverterTemperature3', index=24,
      number=20, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='motorCurrent', full_name='pds.telemetry.motorCurrent', index=25,
      number=21, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='motorVoltage', full_name='pds.telemetry.motorVoltage', index=26,
      number=22, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='inverterVoltages', full_name='pds.telemetry.inverterVoltages', index=27,
      number=23, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='inverterCurrents', full_name='pds.telemetry.inverterCurrents', index=28,
      number=24, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='batteryPackVoltages', full_name='pds.telemetry.batteryPackVoltages', index=29,
      number=26, type=5, cpp_type=1, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='batteryPackCurrent', full_name='pds.telemetry.batteryPackCurrent', index=30,
      number=27, type=5, cpp_type=1, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='batteryCellTemperatures', full_name='pds.telemetry.batteryCellTemperatures', index=31,
      number=28, type=5, cpp_type=1, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='batteryPackMinimumVoltage', full_name='pds.telemetry.batteryPackMinimumVoltage', index=32,
      number=29, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='batteryPackMaxVoltage', full_name='pds.telemetry.batteryPackMaxVoltage', index=33,
      number=30, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='sol1', full_name='pds.telemetry.sol1', index=34,
      number=32, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='sol2', full_name='pds.telemetry.sol2', index=35,
      number=33, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='sol3', full_name='pds.telemetry.sol3', index=36,
      number=34, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='sol4', full_name='pds.telemetry.sol4', index=37,
      number=35, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='sol5', full_name='pds.telemetry.sol5', index=38,
      number=36, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='sol6', full_name='pds.telemetry.sol6', index=39,
      number=37, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='hp', full_name='pds.telemetry.hp', index=40,
      number=38, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='lp1', full_name='pds.telemetry.lp1', index=41,
      number=39, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='lp2', full_name='pds.telemetry.lp2', index=42,
      number=40, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='lp3', full_name='pds.telemetry.lp3', index=43,
      number=41, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='lp4', full_name='pds.telemetry.lp4', index=44,
      number=42, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='pressureVesselTemperature', full_name='pds.telemetry.pressureVesselTemperature', index=45,
      number=43, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='electronicsEnclosurePressure', full_name='pds.telemetry.electronicsEnclosurePressure', index=46,
      number=44, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railDisplacementX', full_name='pds.telemetry.railDisplacementX', index=47,
      number=45, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railDisplacementY', full_name='pds.telemetry.railDisplacementY', index=48,
      number=46, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='tubePressure', full_name='pds.telemetry.tubePressure', index=49,
      number=48, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='coolantPressure1', full_name='pds.telemetry.coolantPressure1', index=50,
      number=49, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='coolantPressure2', full_name='pds.telemetry.coolantPressure2', index=51,
      number=50, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='coolantPressure3', full_name='pds.telemetry.coolantPressure3', index=52,
      number=51, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='coolantTemperature', full_name='pds.telemetry.coolantTemperature', index=53,
      number=52, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railTemperature', full_name='pds.telemetry.railTemperature', index=54,
      number=53, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='pack1Voltage', full_name='pds.telemetry.pack1Voltage', index=55,
      number=55, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='pack2Voltage', full_name='pds.telemetry.pack2Voltage', index=56,
      number=56, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='pack1Current', full_name='pds.telemetry.pack1Current', index=57,
      number=57, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='pack2Current', full_name='pds.telemetry.pack2Current', index=58,
      number=58, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='rail1Voltage', full_name='pds.telemetry.rail1Voltage', index=59,
      number=59, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='rail2Voltage', full_name='pds.telemetry.rail2Voltage', index=60,
      number=60, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='rail3Voltage', full_name='pds.telemetry.rail3Voltage', index=61,
      number=61, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='rail4Voltage', full_name='pds.telemetry.rail4Voltage', index=62,
      number=62, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railVoltageFlag', full_name='pds.telemetry.railVoltageFlag', index=63,
      number=72, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railCurrentSensor1', full_name='pds.telemetry.railCurrentSensor1', index=64,
      number=63, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railCurrentSensor2', full_name='pds.telemetry.railCurrentSensor2', index=65,
      number=64, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railCurrentSensor3', full_name='pds.telemetry.railCurrentSensor3', index=66,
      number=65, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railCurrentSensor4', full_name='pds.telemetry.railCurrentSensor4', index=67,
      number=66, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railCurrentSensor5', full_name='pds.telemetry.railCurrentSensor5', index=68,
      number=67, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railCurrentSensor6', full_name='pds.telemetry.railCurrentSensor6', index=69,
      number=68, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railCurrentSensor7', full_name='pds.telemetry.railCurrentSensor7', index=70,
      number=69, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railCurrentSensor8', full_name='pds.telemetry.railCurrentSensor8', index=71,
      number=70, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='railCurrentFlag', full_name='pds.telemetry.railCurrentFlag', index=72,
      number=73, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=38,
  serialized_end=1970,
)

_TELEMETRY.fields_by_name['podState'].enum_type = States__pb2._PODSTATES
_TELEMETRY.fields_by_name['breakNodeState'].enum_type = States__pb2._BRAKENODESTATES
_TELEMETRY.fields_by_name['motorState'].enum_type = States__pb2._MOTORSTATES
_TELEMETRY.fields_by_name['controlsInterfaceState'].enum_type = States__pb2._CONTROLSINTERFACESTATES
_TELEMETRY.fields_by_name['inverterState'].enum_type = States__pb2._INVERTERSTATES
_TELEMETRY.fields_by_name['lvdcNodeState'].enum_type = States__pb2._LVDCNODESTATES
_TELEMETRY.fields_by_name['batteryManagementState'].enum_type = States__pb2._BATTERYMANAGEMENTSTATES
_TELEMETRY.fields_by_name['breakNodePerceivedPodState'].enum_type = States__pb2._PODSTATES
_TELEMETRY.fields_by_name['rearNodeState'].enum_type = States__pb2._REARNODESTATES
DESCRIPTOR.message_types_by_name['telemetry'] = _TELEMETRY
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

telemetry = _reflection.GeneratedProtocolMessageType('telemetry', (_message.Message,), dict(
  DESCRIPTOR = _TELEMETRY,
  __module__ = 'PodTelem_pb2'
  # @@protoc_insertion_point(class_scope:pds.telemetry)
  ))
_sym_db.RegisterMessage(telemetry)


# @@protoc_insertion_point(module_scope)
