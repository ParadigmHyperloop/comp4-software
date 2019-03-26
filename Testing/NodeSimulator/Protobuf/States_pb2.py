# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: States.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='States.proto',
  package='',
  syntax='proto2',
  serialized_options=None,
  serialized_pb=_b('\n\x0cStates.proto*\x9d\x01\n\x0f\x42rakeNodeStates\x12\x0e\n\nbnsBooting\x10\x00\x12\x0e\n\nbnsStandby\x10\x01\x12\r\n\tbnsArming\x10\x02\x12\x0c\n\x08\x62nsArmed\x10\x03\x12\r\n\tbnsFlight\x10\x04\x12\x0e\n\nbnsBraking\x10\x05\x12\x0e\n\nbnsVenting\x10\x06\x12\x10\n\x0c\x62nsRetrieval\x10\x07\x12\x0c\n\x08\x62nsError\x10\x08*\xbb\x01\n\tPodStates\x12\r\n\tpsBooting\x10\x00\x12\r\n\tpsStandby\x10\x01\x12\x0c\n\x08psArming\x10\x02\x12\x0b\n\x07psArmed\x10\x03\x12\x0f\n\x0bpsPreFlight\x10\x04\x12\x12\n\x0epsAcceleration\x10\x05\x12\x0e\n\npsCoasting\x10\x06\x12\r\n\tpsBraking\x10\x07\x12\x0f\n\x0bpsDisarming\x10\x08\x12\x0f\n\x0bpsRetrieval\x10\t\x12\x0f\n\x0bpsEmergency\x10\n*C\n\x0bMotorStates\x12\t\n\x05msOff\x10\x00\x12\x0e\n\nmsCharging\x10\x01\x12\n\n\x06msIdle\x10\x02\x12\r\n\tmsEngaged\x10\x03**\n\x17\x43ontrolsInterfaceStates\x12\x0f\n\x0b\x63iConnected\x10\x00* \n\x0eInverterStates\x12\x0e\n\niConnected\x10\x00*)\n\x17\x42\x61tteryManagementStates\x12\x0e\n\nbmsNominal\x10\x00*.\n\x0eRearNodeStates\x12\r\n\trnBooting\x10\x00\x12\r\n\trnNominal\x10\x01*!\n\x0eLvdcNodeStates\x12\x0f\n\x0blvdcBooting\x10\x00')
)

_BRAKENODESTATES = _descriptor.EnumDescriptor(
  name='BrakeNodeStates',
  full_name='BrakeNodeStates',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='bnsBooting', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='bnsStandby', index=1, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='bnsArming', index=2, number=2,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='bnsArmed', index=3, number=3,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='bnsFlight', index=4, number=4,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='bnsBraking', index=5, number=5,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='bnsVenting', index=6, number=6,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='bnsRetrieval', index=7, number=7,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='bnsError', index=8, number=8,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=17,
  serialized_end=174,
)
_sym_db.RegisterEnumDescriptor(_BRAKENODESTATES)

BrakeNodeStates = enum_type_wrapper.EnumTypeWrapper(_BRAKENODESTATES)
_PODSTATES = _descriptor.EnumDescriptor(
  name='PodStates',
  full_name='PodStates',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='psBooting', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psStandby', index=1, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psArming', index=2, number=2,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psArmed', index=3, number=3,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psPreFlight', index=4, number=4,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psAcceleration', index=5, number=5,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psCoasting', index=6, number=6,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psBraking', index=7, number=7,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psDisarming', index=8, number=8,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psRetrieval', index=9, number=9,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psEmergency', index=10, number=10,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=177,
  serialized_end=364,
)
_sym_db.RegisterEnumDescriptor(_PODSTATES)

PodStates = enum_type_wrapper.EnumTypeWrapper(_PODSTATES)
_MOTORSTATES = _descriptor.EnumDescriptor(
  name='MotorStates',
  full_name='MotorStates',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='msOff', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='msCharging', index=1, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='msIdle', index=2, number=2,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='msEngaged', index=3, number=3,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=366,
  serialized_end=433,
)
_sym_db.RegisterEnumDescriptor(_MOTORSTATES)

MotorStates = enum_type_wrapper.EnumTypeWrapper(_MOTORSTATES)
_CONTROLSINTERFACESTATES = _descriptor.EnumDescriptor(
  name='ControlsInterfaceStates',
  full_name='ControlsInterfaceStates',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='ciConnected', index=0, number=0,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=435,
  serialized_end=477,
)
_sym_db.RegisterEnumDescriptor(_CONTROLSINTERFACESTATES)

ControlsInterfaceStates = enum_type_wrapper.EnumTypeWrapper(_CONTROLSINTERFACESTATES)
_INVERTERSTATES = _descriptor.EnumDescriptor(
  name='InverterStates',
  full_name='InverterStates',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='iConnected', index=0, number=0,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=479,
  serialized_end=511,
)
_sym_db.RegisterEnumDescriptor(_INVERTERSTATES)

InverterStates = enum_type_wrapper.EnumTypeWrapper(_INVERTERSTATES)
_BATTERYMANAGEMENTSTATES = _descriptor.EnumDescriptor(
  name='BatteryManagementStates',
  full_name='BatteryManagementStates',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='bmsNominal', index=0, number=0,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=513,
  serialized_end=554,
)
_sym_db.RegisterEnumDescriptor(_BATTERYMANAGEMENTSTATES)

BatteryManagementStates = enum_type_wrapper.EnumTypeWrapper(_BATTERYMANAGEMENTSTATES)
_REARNODESTATES = _descriptor.EnumDescriptor(
  name='RearNodeStates',
  full_name='RearNodeStates',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='rnBooting', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='rnNominal', index=1, number=1,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=556,
  serialized_end=602,
)
_sym_db.RegisterEnumDescriptor(_REARNODESTATES)

RearNodeStates = enum_type_wrapper.EnumTypeWrapper(_REARNODESTATES)
_LVDCNODESTATES = _descriptor.EnumDescriptor(
  name='LvdcNodeStates',
  full_name='LvdcNodeStates',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='lvdcBooting', index=0, number=0,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=604,
  serialized_end=637,
)
_sym_db.RegisterEnumDescriptor(_LVDCNODESTATES)

LvdcNodeStates = enum_type_wrapper.EnumTypeWrapper(_LVDCNODESTATES)
bnsBooting = 0
bnsStandby = 1
bnsArming = 2
bnsArmed = 3
bnsFlight = 4
bnsBraking = 5
bnsVenting = 6
bnsRetrieval = 7
bnsError = 8
psBooting = 0
psStandby = 1
psArming = 2
psArmed = 3
psPreFlight = 4
psAcceleration = 5
psCoasting = 6
psBraking = 7
psDisarming = 8
psRetrieval = 9
psEmergency = 10
msOff = 0
msCharging = 1
msIdle = 2
msEngaged = 3
ciConnected = 0
iConnected = 0
bmsNominal = 0
rnBooting = 0
rnNominal = 1
lvdcBooting = 0


DESCRIPTOR.enum_types_by_name['BrakeNodeStates'] = _BRAKENODESTATES
DESCRIPTOR.enum_types_by_name['PodStates'] = _PODSTATES
DESCRIPTOR.enum_types_by_name['MotorStates'] = _MOTORSTATES
DESCRIPTOR.enum_types_by_name['ControlsInterfaceStates'] = _CONTROLSINTERFACESTATES
DESCRIPTOR.enum_types_by_name['InverterStates'] = _INVERTERSTATES
DESCRIPTOR.enum_types_by_name['BatteryManagementStates'] = _BATTERYMANAGEMENTSTATES
DESCRIPTOR.enum_types_by_name['RearNodeStates'] = _REARNODESTATES
DESCRIPTOR.enum_types_by_name['LvdcNodeStates'] = _LVDCNODESTATES
_sym_db.RegisterFileDescriptor(DESCRIPTOR)


# @@protoc_insertion_point(module_scope)