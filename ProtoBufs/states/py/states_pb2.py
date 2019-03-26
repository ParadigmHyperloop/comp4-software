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
  serialized_pb=_b('\n\x0cstates.proto*\x9d\x01\n\x0f\x62reakNodeStates\x12\x0e\n\nbnsBooting\x10\x00\x12\x0e\n\nbnsStandby\x10\x01\x12\r\n\tbnsArming\x10\x02\x12\x0c\n\x08\x62nsArmed\x10\x03\x12\r\n\tbnsFlight\x10\x04\x12\x0e\n\nbnsBraking\x10\x05\x12\x0e\n\nbnsVenting\x10\x06\x12\x10\n\x0c\x62nsRetrieval\x10\x07\x12\x0c\n\x08\x62nsError\x10\x08*\x8e\x01\n\tpodStates\x12\r\n\tpsBooting\x10\x00\x12\r\n\tpsStandby\x10\x01\x12\x0c\n\x08psArming\x10\x02\x12\x0b\n\x07psArmed\x10\x03\x12\x0c\n\x08psFlight\x10\x04\x12\r\n\tpsBraking\x10\x05\x12\r\n\tpsVenting\x10\x06\x12\x0f\n\x0bpsRetrieval\x10\x07\x12\x0b\n\x07psError\x10\x08*C\n\x0bmotorStates\x12\t\n\x05msOff\x10\x00\x12\x0e\n\nmsCharging\x10\x01\x12\n\n\x06msIdle\x10\x02\x12\r\n\tmsEngaged\x10\x03**\n\x17\x63ontrolsInterfaceStates\x12\x0f\n\x0b\x63iConnected\x10\x00* \n\x0einverterStates\x12\x0e\n\niConnected\x10\x00*)\n\x17\x62\x61tteryManagementStates\x12\x0e\n\nbmsNominal\x10\x00*.\n\x0erearNodeStates\x12\r\n\trnBooting\x10\x00\x12\r\n\trnNominal\x10\x01*!\n\x0elvdcNodeStates\x12\x0f\n\x0blvdcBooting\x10\x00')
)

_BREAKNODESTATES = _descriptor.EnumDescriptor(
  name='breakNodeStates',
  full_name='breakNodeStates',
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
_sym_db.RegisterEnumDescriptor(_BREAKNODESTATES)

breakNodeStates = enum_type_wrapper.EnumTypeWrapper(_BREAKNODESTATES)
_PODSTATES = _descriptor.EnumDescriptor(
  name='podStates',
  full_name='podStates',
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
      name='psFlight', index=4, number=4,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psBraking', index=5, number=5,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psVenting', index=6, number=6,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psRetrieval', index=7, number=7,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='psError', index=8, number=8,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=177,
  serialized_end=319,
)
_sym_db.RegisterEnumDescriptor(_PODSTATES)

podStates = enum_type_wrapper.EnumTypeWrapper(_PODSTATES)
_MOTORSTATES = _descriptor.EnumDescriptor(
  name='motorStates',
  full_name='motorStates',
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
  serialized_start=321,
  serialized_end=388,
)
_sym_db.RegisterEnumDescriptor(_MOTORSTATES)

motorStates = enum_type_wrapper.EnumTypeWrapper(_MOTORSTATES)
_CONTROLSINTERFACESTATES = _descriptor.EnumDescriptor(
  name='controlsInterfaceStates',
  full_name='controlsInterfaceStates',
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
  serialized_start=390,
  serialized_end=432,
)
_sym_db.RegisterEnumDescriptor(_CONTROLSINTERFACESTATES)

controlsInterfaceStates = enum_type_wrapper.EnumTypeWrapper(_CONTROLSINTERFACESTATES)
_INVERTERSTATES = _descriptor.EnumDescriptor(
  name='inverterStates',
  full_name='inverterStates',
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
  serialized_start=434,
  serialized_end=466,
)
_sym_db.RegisterEnumDescriptor(_INVERTERSTATES)

inverterStates = enum_type_wrapper.EnumTypeWrapper(_INVERTERSTATES)
_BATTERYMANAGEMENTSTATES = _descriptor.EnumDescriptor(
  name='batteryManagementStates',
  full_name='batteryManagementStates',
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
  serialized_start=468,
  serialized_end=509,
)
_sym_db.RegisterEnumDescriptor(_BATTERYMANAGEMENTSTATES)

batteryManagementStates = enum_type_wrapper.EnumTypeWrapper(_BATTERYMANAGEMENTSTATES)
_REARNODESTATES = _descriptor.EnumDescriptor(
  name='rearNodeStates',
  full_name='rearNodeStates',
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
  serialized_start=511,
  serialized_end=557,
)
_sym_db.RegisterEnumDescriptor(_REARNODESTATES)

rearNodeStates = enum_type_wrapper.EnumTypeWrapper(_REARNODESTATES)
_LVDCNODESTATES = _descriptor.EnumDescriptor(
  name='lvdcNodeStates',
  full_name='lvdcNodeStates',
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
  serialized_start=559,
  serialized_end=592,
)
_sym_db.RegisterEnumDescriptor(_LVDCNODESTATES)

lvdcNodeStates = enum_type_wrapper.EnumTypeWrapper(_LVDCNODESTATES)
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
psFlight = 4
psBraking = 5
psVenting = 6
psRetrieval = 7
psError = 8
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


DESCRIPTOR.enum_types_by_name['breakNodeStates'] = _BREAKNODESTATES
DESCRIPTOR.enum_types_by_name['podStates'] = _PODSTATES
DESCRIPTOR.enum_types_by_name['motorStates'] = _MOTORSTATES
DESCRIPTOR.enum_types_by_name['controlsInterfaceStates'] = _CONTROLSINTERFACESTATES
DESCRIPTOR.enum_types_by_name['inverterStates'] = _INVERTERSTATES
DESCRIPTOR.enum_types_by_name['batteryManagementStates'] = _BATTERYMANAGEMENTSTATES
DESCRIPTOR.enum_types_by_name['rearNodeStates'] = _REARNODESTATES
DESCRIPTOR.enum_types_by_name['lvdcNodeStates'] = _LVDCNODESTATES
_sym_db.RegisterFileDescriptor(DESCRIPTOR)


# @@protoc_insertion_point(module_scope)