/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.3 at Wed Apr 24 16:00:51 2019. */

#ifndef PB_PARADIGM_PB_H_INCLUDED
#define PB_PARADIGM_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _BrakeNodeStates {
    BrakeNodeStates_bnsBooting = 0,
    BrakeNodeStates_bnsStandby = 1,
    BrakeNodeStates_bnsArming = 2,
    BrakeNodeStates_bnsArmed = 3,
    BrakeNodeStates_bnsFlight = 4,
    BrakeNodeStates_bnsBraking = 5,
    BrakeNodeStates_bnsVenting = 6,
    BrakeNodeStates_bnsRetrieval = 7,
    BrakeNodeStates_bnsError = 8
} BrakeNodeStates;
#define _BrakeNodeStates_MIN BrakeNodeStates_bnsBooting
#define _BrakeNodeStates_MAX BrakeNodeStates_bnsError
#define _BrakeNodeStates_ARRAYSIZE ((BrakeNodeStates)(BrakeNodeStates_bnsError+1))

typedef enum _PodStates {
    PodStates_psBooting = 0,
    PodStates_psStandby = 1,
    PodStates_psArming = 2,
    PodStates_psArmed = 3,
    PodStates_psPreFlight = 4,
    PodStates_psAcceleration = 5,
    PodStates_psCoasting = 6,
    PodStates_psBraking = 7,
    PodStates_psDisarming = 8,
    PodStates_psRetrieval = 9,
    PodStates_psEmergency = 10
} PodStates;
#define _PodStates_MIN PodStates_psBooting
#define _PodStates_MAX PodStates_psEmergency
#define _PodStates_ARRAYSIZE ((PodStates)(PodStates_psEmergency+1))

typedef enum _MotorStates {
    MotorStates_msOff = 0,
    MotorStates_msCharging = 1,
    MotorStates_msIdle = 2,
    MotorStates_msEngaged = 3
} MotorStates;
#define _MotorStates_MIN MotorStates_msOff
#define _MotorStates_MAX MotorStates_msEngaged
#define _MotorStates_ARRAYSIZE ((MotorStates)(MotorStates_msEngaged+1))

typedef enum _ControlsInterfaceStates {
    ControlsInterfaceStates_ciArm = 0,
    ControlsInterfaceStates_ciFlight = 1,
    ControlsInterfaceStates_ciEmergencyStop = 2
} ControlsInterfaceStates;
#define _ControlsInterfaceStates_MIN ControlsInterfaceStates_ciArm
#define _ControlsInterfaceStates_MAX ControlsInterfaceStates_ciEmergencyStop
#define _ControlsInterfaceStates_ARRAYSIZE ((ControlsInterfaceStates)(ControlsInterfaceStates_ciEmergencyStop+1))

typedef enum _InverterStates {
    InverterStates_iConnected = 0
} InverterStates;
#define _InverterStates_MIN InverterStates_iConnected
#define _InverterStates_MAX InverterStates_iConnected
#define _InverterStates_ARRAYSIZE ((InverterStates)(InverterStates_iConnected+1))

typedef enum _BatteryManagementStates {
    BatteryManagementStates_bmsNominal = 0
} BatteryManagementStates;
#define _BatteryManagementStates_MIN BatteryManagementStates_bmsNominal
#define _BatteryManagementStates_MAX BatteryManagementStates_bmsNominal
#define _BatteryManagementStates_ARRAYSIZE ((BatteryManagementStates)(BatteryManagementStates_bmsNominal+1))

typedef enum _RearNodeStates {
    RearNodeStates_rnBooting = 0,
    RearNodeStates_rnNominal = 1
} RearNodeStates;
#define _RearNodeStates_MIN RearNodeStates_rnBooting
#define _RearNodeStates_MAX RearNodeStates_rnNominal
#define _RearNodeStates_ARRAYSIZE ((RearNodeStates)(RearNodeStates_rnNominal+1))

typedef enum _LvdcNodeStates {
    LvdcNodeStates_lvdcBooting = 0
} LvdcNodeStates;
#define _LvdcNodeStates_MIN LvdcNodeStates_lvdcBooting
#define _LvdcNodeStates_MAX LvdcNodeStates_lvdcBooting
#define _LvdcNodeStates_ARRAYSIZE ((LvdcNodeStates)(LvdcNodeStates_lvdcBooting+1))

/* Struct definitions */
typedef struct _DtsNodeToFc {
    bool has_brakeNodeState;
    BrakeNodeStates brakeNodeState;
    bool has_brakeSolenoidState;
    bool brakeSolenoidState;
    bool has_ventSolenoidState;
    bool ventSolenoidState;
    bool has_rotorTemperature;
    float rotorTemperature;
    bool has_pneumaticTemperature;
    float pneumaticTemperature;
    bool has_tankPressure;
    float tankPressure;
    bool has_brakePressure;
    float brakePressure;
/* @@protoc_insertion_point(struct:DtsNodeToFc) */
} DtsNodeToFc;

typedef struct _FcToBrakeNode {
    bool has_podState;
    PodStates podState;
    bool has_manualNodeState;
    BrakeNodeStates manualNodeState;
/* @@protoc_insertion_point(struct:FcToBrakeNode) */
} FcToBrakeNode;

typedef struct _brakeNodeData {
    int32_t id;
    bool has_sol1;
    bool sol1;
    bool has_sol2;
    bool sol2;
    bool has_sol3;
    bool sol3;
    bool has_sol4;
    bool sol4;
    bool has_sol5;
    bool sol5;
    bool has_sol6;
    bool sol6;
    bool has_hp;
    int32_t hp;
    bool has_lp1;
    int32_t lp1;
    bool has_lp2;
    int32_t lp2;
    bool has_lp3;
    int32_t lp3;
    bool has_lp4;
    int32_t lp4;
    bool has_temp;
    int32_t temp;
    bool has_state;
    BrakeNodeStates state;
/* @@protoc_insertion_point(struct:brakeNodeData) */
} brakeNodeData;

typedef struct _podCommand {
    bool has_controlsInterfaceState;
    ControlsInterfaceStates controlsInterfaceState;
    bool has_manualPodState;
    PodStates manualPodState;
    bool has_manualBrakeNodeState;
    BrakeNodeStates manualBrakeNodeState;
    bool has_manualMotorState;
    MotorStates manualMotorState;
    bool has_manualLvdcNodeState;
    LvdcNodeStates manualLvdcNodeState;
    bool has_automaticStateTransitions;
    bool automaticStateTransitions;
/* @@protoc_insertion_point(struct:podCommand) */
} podCommand;

typedef struct _telemetry {
    bool has_podState;
    PodStates podState;
    bool has_breakNodeState;
    BrakeNodeStates breakNodeState;
    bool has_motorState;
    MotorStates motorState;
    bool has_controlsInterfaceState;
    ControlsInterfaceStates controlsInterfaceState;
    bool has_inverterState;
    InverterStates inverterState;
    bool has_missionTime;
    int32_t missionTime;
    bool has_flightTime;
    int32_t flightTime;
    pb_callback_t heartbeatFlags;
    bool has_podPosition;
    int32_t podPosition;
    bool has_podVelocity;
    int32_t podVelocity;
    bool has_podAccelerationX;
    int32_t podAccelerationX;
    bool has_podAccelerationY;
    int32_t podAccelerationY;
    bool has_podAccelerationZ;
    int32_t podAccelerationZ;
    bool has_imuVelocity;
    int32_t imuVelocity;
    bool has_imuAccelerationX;
    int32_t imuAccelerationX;
    bool has_tachometerVelocity;
    int32_t tachometerVelocity;
    bool has_resolverVelocity;
    int32_t resolverVelocity;
    bool has_inverterTemperature1;
    int32_t inverterTemperature1;
    bool has_inverterTemperature2;
    int32_t inverterTemperature2;
    bool has_inverterTemperature3;
    int32_t inverterTemperature3;
    bool has_motorCurrent;
    int32_t motorCurrent;
    bool has_motorVoltage;
    int32_t motorVoltage;
    bool has_inverterVoltages;
    int32_t inverterVoltages;
    bool has_inverterCurrents;
    int32_t inverterCurrents;
    bool has_batteryManagementState;
    BatteryManagementStates batteryManagementState;
    pb_callback_t batteryPackVoltages;
    pb_callback_t batteryPackCurrent;
    pb_callback_t batteryCellTemperatures;
    bool has_batteryPackMinimumVoltage;
    int32_t batteryPackMinimumVoltage;
    bool has_batteryPackMaxVoltage;
    int32_t batteryPackMaxVoltage;
    bool has_breakNodePerceivedPodState;
    PodStates breakNodePerceivedPodState;
    bool has_sol1;
    bool sol1;
    bool has_sol2;
    bool sol2;
    bool has_sol3;
    bool sol3;
    bool has_sol4;
    bool sol4;
    bool has_sol5;
    bool sol5;
    bool has_sol6;
    bool sol6;
    bool has_hp;
    int32_t hp;
    bool has_lp1;
    int32_t lp1;
    bool has_lp2;
    int32_t lp2;
    bool has_lp3;
    int32_t lp3;
    bool has_lp4;
    int32_t lp4;
    bool has_pressureVesselTemperature;
    int32_t pressureVesselTemperature;
    bool has_electronicsEnclosurePressure;
    int32_t electronicsEnclosurePressure;
    bool has_railDisplacementX;
    int32_t railDisplacementX;
    bool has_railDisplacementY;
    int32_t railDisplacementY;
    bool has_tubePressure;
    int32_t tubePressure;
    bool has_coolantPressure1;
    int32_t coolantPressure1;
    bool has_coolantPressure2;
    int32_t coolantPressure2;
    bool has_coolantPressure3;
    int32_t coolantPressure3;
    bool has_coolantTemperature;
    int32_t coolantTemperature;
    bool has_railTemperature;
    int32_t railTemperature;
    bool has_lvdcNodeState;
    LvdcNodeStates lvdcNodeState;
    bool has_pack1Voltage;
    int32_t pack1Voltage;
    bool has_pack2Voltage;
    int32_t pack2Voltage;
    bool has_pack1Current;
    int32_t pack1Current;
    bool has_pack2Current;
    int32_t pack2Current;
    bool has_rail1Voltage;
    int32_t rail1Voltage;
    bool has_rail2Voltage;
    int32_t rail2Voltage;
    bool has_rail3Voltage;
    int32_t rail3Voltage;
    bool has_rail4Voltage;
    int32_t rail4Voltage;
    bool has_railCurrentSensor1;
    int32_t railCurrentSensor1;
    bool has_railCurrentSensor2;
    int32_t railCurrentSensor2;
    bool has_railCurrentSensor3;
    int32_t railCurrentSensor3;
    bool has_railCurrentSensor4;
    int32_t railCurrentSensor4;
    bool has_railCurrentSensor5;
    int32_t railCurrentSensor5;
    bool has_railCurrentSensor6;
    int32_t railCurrentSensor6;
    bool has_railCurrentSensor7;
    int32_t railCurrentSensor7;
    bool has_railCurrentSensor8;
    int32_t railCurrentSensor8;
    pb_callback_t heartbeatValues;
    bool has_railVoltageFlag;
    int32_t railVoltageFlag;
    bool has_railCurrentFlag;
    int32_t railCurrentFlag;
/* @@protoc_insertion_point(struct:telemetry) */
} telemetry;

/* Default values for struct fields */
extern const BrakeNodeStates DtsNodeToFc_brakeNodeState_default;
extern const bool DtsNodeToFc_brakeSolenoidState_default;
extern const bool DtsNodeToFc_ventSolenoidState_default;

/* Initializer values for message structs */
#define brakeNodeData_init_default               {0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, _BrakeNodeStates_MIN}
#define podCommand_init_default                  {false, _ControlsInterfaceStates_MIN, false, _PodStates_MIN, false, _BrakeNodeStates_MIN, false, _MotorStates_MIN, false, _LvdcNodeStates_MIN, false, 0}
#define telemetry_init_default                   {false, _PodStates_MIN, false, _BrakeNodeStates_MIN, false, _MotorStates_MIN, false, _ControlsInterfaceStates_MIN, false, _InverterStates_MIN, false, 0, false, 0, {{NULL}, NULL}, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, _BatteryManagementStates_MIN, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, false, 0, false, 0, false, _PodStates_MIN, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, _LvdcNodeStates_MIN, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, {{NULL}, NULL}, false, 0, false, 0}
#define FcToBrakeNode_init_default               {false, _PodStates_MIN, false, _BrakeNodeStates_MIN}
#define DtsNodeToFc_init_default                 {false, BrakeNodeStates_bnsFlight, false, false, false, false, false, 0, false, 0, false, 0, false, 0}
#define brakeNodeData_init_zero                  {0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, _BrakeNodeStates_MIN}
#define podCommand_init_zero                     {false, _ControlsInterfaceStates_MIN, false, _PodStates_MIN, false, _BrakeNodeStates_MIN, false, _MotorStates_MIN, false, _LvdcNodeStates_MIN, false, 0}
#define telemetry_init_zero                      {false, _PodStates_MIN, false, _BrakeNodeStates_MIN, false, _MotorStates_MIN, false, _ControlsInterfaceStates_MIN, false, _InverterStates_MIN, false, 0, false, 0, {{NULL}, NULL}, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, _BatteryManagementStates_MIN, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, false, 0, false, 0, false, _PodStates_MIN, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, _LvdcNodeStates_MIN, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, {{NULL}, NULL}, false, 0, false, 0}
#define FcToBrakeNode_init_zero                  {false, _PodStates_MIN, false, _BrakeNodeStates_MIN}
#define DtsNodeToFc_init_zero                    {false, _BrakeNodeStates_MIN, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0}

/* Field tags (for use in manual encoding/decoding) */
#define DtsNodeToFc_brakeNodeState_tag           1
#define DtsNodeToFc_brakeSolenoidState_tag       2
#define DtsNodeToFc_ventSolenoidState_tag        3
#define DtsNodeToFc_rotorTemperature_tag         4
#define DtsNodeToFc_pneumaticTemperature_tag     5
#define DtsNodeToFc_tankPressure_tag             6
#define DtsNodeToFc_brakePressure_tag            7
#define FcToBrakeNode_podState_tag               1
#define FcToBrakeNode_manualNodeState_tag        2
#define brakeNodeData_id_tag                     1
#define brakeNodeData_state_tag                  14
#define brakeNodeData_sol1_tag                   2
#define brakeNodeData_sol2_tag                   3
#define brakeNodeData_sol3_tag                   4
#define brakeNodeData_sol4_tag                   5
#define brakeNodeData_sol5_tag                   6
#define brakeNodeData_sol6_tag                   7
#define brakeNodeData_hp_tag                     8
#define brakeNodeData_lp1_tag                    9
#define brakeNodeData_lp2_tag                    10
#define brakeNodeData_lp3_tag                    11
#define brakeNodeData_lp4_tag                    12
#define brakeNodeData_temp_tag                   13
#define podCommand_controlsInterfaceState_tag    1
#define podCommand_manualPodState_tag            2
#define podCommand_manualBrakeNodeState_tag      3
#define podCommand_manualMotorState_tag          4
#define podCommand_manualLvdcNodeState_tag       5
#define podCommand_automaticStateTransitions_tag 6
#define telemetry_podState_tag                   1
#define telemetry_breakNodeState_tag             2
#define telemetry_motorState_tag                 3
#define telemetry_controlsInterfaceState_tag     4
#define telemetry_inverterState_tag              5
#define telemetry_lvdcNodeState_tag              54
#define telemetry_batteryManagementState_tag     25
#define telemetry_breakNodePerceivedPodState_tag 31
#define telemetry_missionTime_tag                6
#define telemetry_flightTime_tag                 7
#define telemetry_heartbeatFlags_tag             8
#define telemetry_heartbeatValues_tag            71
#define telemetry_podPosition_tag                9
#define telemetry_podVelocity_tag                10
#define telemetry_podAccelerationX_tag           11
#define telemetry_podAccelerationY_tag           12
#define telemetry_podAccelerationZ_tag           13
#define telemetry_imuVelocity_tag                14
#define telemetry_imuAccelerationX_tag           15
#define telemetry_tachometerVelocity_tag         16
#define telemetry_resolverVelocity_tag           17
#define telemetry_inverterTemperature1_tag       18
#define telemetry_inverterTemperature2_tag       19
#define telemetry_inverterTemperature3_tag       20
#define telemetry_motorCurrent_tag               21
#define telemetry_motorVoltage_tag               22
#define telemetry_inverterVoltages_tag           23
#define telemetry_inverterCurrents_tag           24
#define telemetry_batteryPackVoltages_tag        26
#define telemetry_batteryPackCurrent_tag         27
#define telemetry_batteryCellTemperatures_tag    28
#define telemetry_batteryPackMinimumVoltage_tag  29
#define telemetry_batteryPackMaxVoltage_tag      30
#define telemetry_sol1_tag                       32
#define telemetry_sol2_tag                       33
#define telemetry_sol3_tag                       34
#define telemetry_sol4_tag                       35
#define telemetry_sol5_tag                       36
#define telemetry_sol6_tag                       37
#define telemetry_hp_tag                         38
#define telemetry_lp1_tag                        39
#define telemetry_lp2_tag                        40
#define telemetry_lp3_tag                        41
#define telemetry_lp4_tag                        42
#define telemetry_pressureVesselTemperature_tag  43
#define telemetry_electronicsEnclosurePressure_tag 44
#define telemetry_railDisplacementX_tag          45
#define telemetry_railDisplacementY_tag          46
#define telemetry_tubePressure_tag               48
#define telemetry_coolantPressure1_tag           49
#define telemetry_coolantPressure2_tag           50
#define telemetry_coolantPressure3_tag           51
#define telemetry_coolantTemperature_tag         52
#define telemetry_railTemperature_tag            53
#define telemetry_pack1Voltage_tag               55
#define telemetry_pack2Voltage_tag               56
#define telemetry_pack1Current_tag               57
#define telemetry_pack2Current_tag               58
#define telemetry_rail1Voltage_tag               59
#define telemetry_rail2Voltage_tag               60
#define telemetry_rail3Voltage_tag               61
#define telemetry_rail4Voltage_tag               62
#define telemetry_railVoltageFlag_tag            72
#define telemetry_railCurrentSensor1_tag         63
#define telemetry_railCurrentSensor2_tag         64
#define telemetry_railCurrentSensor3_tag         65
#define telemetry_railCurrentSensor4_tag         66
#define telemetry_railCurrentSensor5_tag         67
#define telemetry_railCurrentSensor6_tag         68
#define telemetry_railCurrentSensor7_tag         69
#define telemetry_railCurrentSensor8_tag         70
#define telemetry_railCurrentFlag_tag            73

/* Struct field encoding specification for nanopb */
extern const pb_field_t brakeNodeData_fields[15];
extern const pb_field_t podCommand_fields[7];
extern const pb_field_t telemetry_fields[73];
extern const pb_field_t FcToBrakeNode_fields[3];
extern const pb_field_t DtsNodeToFc_fields[8];

/* Maximum encoded size of messages (where known) */
#define brakeNodeData_size                       91
#define podCommand_size                          12
/* telemetry_size depends on runtime parameters */
#define FcToBrakeNode_size                       4
#define DtsNodeToFc_size                         26

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define PARADIGM_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
