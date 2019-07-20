
#ifndef FLIGHTCOMPUTER_CONSTANTS_H
#define FLIGHTCOMPUTER_CONSTANTS_H

namespace GENERAL_CONSTANTS
{
    static const uint32_t VACUUM = 0;
    static const uint32_t ATMOSPHERE = 15;

    static const uint32_t CLOSED = 0;
    static const uint32_t OPEN = 1;

    static const int32_t FLAGS_GOOD = -1;

    static const int32_t HV_CELL_COUNT = 194;

    static const float   REAR_WHEEL_CIRCUMFRENCE = 0.933619f;
    static const float   STRIP_DISTANCE = 30.48;
    static const float   DEFAULT_MAX_STRIP_COUNT = 80;

    static const int32_t NAV_SERIAL_MESSAGE_SIZE = 11;
    static const float   STATIONARY_THRSHOLD_M = 0.5f;
};

namespace CONNECTION_FLAGS
{
    static const uint32_t TOTAL_CONNECTION_COUNT = 6;  // update if you add
    static const uint32_t BRAKE_NODE_HEARTBEAT_INDEX = 0;
    static const uint32_t LVDC_NODE_HEARTBEAT_INDEX = 1;
    static const uint32_t BMS_HEARTBEAT_INDEX = 2;
    static const uint32_t INTERFACE_HEARTBEAT_INDEX = 3;
    static const uint32_t ENCLOSURE_HEARTBEAT_INDEX = 4;
    static const uint32_t NAVIGATION_HEARTBEAT_INDEX = 5;
};

// Inverter Sensor Flag Indices
namespace INVERTER_FLAGS
{
    static const uint32_t INVERTER_SENSOR_COUNT = 5;  // update if you add
    static const uint32_t MOTOR_TEMPERATURE_INDEX = 0;
    static const uint32_t BUS_VOLTAGE_INDEX = 1;
    static const uint32_t GATE_DRIVER_TEMP_INDEX = 2;
    static const uint32_t CONTROL_BOARD_TEMP_INDEX = 3;
    static const uint32_t MAX_IGBT_INDEX = 4;

    static const int32_t INVERTER_FAULT_COUNT = 4;
    static const int32_t POST_FAULT_LO_INDEX = 0;
    static const int32_t POST_FAULT_HI_INDEX = 1;
    static const int32_t RUN_FAULT_LO_INDEX = 2;
    static const int32_t RUN_FAULT_HI_INDEX = 3;
};

// Inverter Hi Fault mapping
namespace INVERTER_RUN_FAULT_HI
{
    static const uint32_t RESOLVER_NOT_CONNECTED = 0x4000;
}

// BMS Sensor Flag Indices
namespace BMS_FLAGS
{
    static const uint32_t BMS_SENSOR_COUNT = 9;  // update if you add
    static const uint32_t HV_PACK_VOLTAGE_INDEX = 0;
    static const uint32_t HV_MAX_CELL_TEMP_INDEX = 1;
    static const uint32_t HV_SOC_INDEX = 2;

    static const uint32_t LV1_PACK_VOLTAGE_INDEX = 3;
    static const uint32_t LV1_CELL_TEMP_INDEX = 4;
    static const uint32_t LV1_SOC_INDEX = 5;

    static const uint32_t LV2_PACK_VOLTAGE_INDEX = 6;
    static const uint32_t LV2_CELL_TEMP_INDEX = 7;
    static const uint32_t LV2_SOC_INDEX = 8;
};

// Node Sensor Flag indices
namespace NODE_FLAGS
{
    static const uint32_t NODE_SENSOR_COUNT = 10; // update if you add
    static const uint32_t LP1_INDEX = 0;
    static const uint32_t LP2_INDEX = 1;
    static const uint32_t LP3_INDEX = 2;
    static const uint32_t LP4_INDEX = 3;
    static const uint32_t HP_INDEX = 4;
    static const uint32_t HP_TEMP_INDEX = 5;
    static const uint32_t ENCLOSURE_PRESSURE_INDEX = 6;
    static const uint32_t ENCLOSURE_TEMPERATURE_INDEX = 7;
    static const uint32_t COOLING_PRESSURE_INDEX = 8;
    static const uint32_t COOLING_TEMPERATURE_INDEX = 9;
};

// Solenoid Status indices
namespace SOLENOID_INDEX
{
    static const uint32_t SOL1 = 0;  // update if you add
    static const uint32_t SOL2 = 1;
    static const uint32_t SOL3 = 2;
    static const uint32_t SOL4 = 3;
};

namespace CAN_IDS
{
    static const uint32_t TEMP1_10HZ         = 0x0A0;
    static const uint32_t TEMP2_10HZ         = 0x0A1;
    static const uint32_t TEMP3_10HZ         = 0x0A2;
    static const uint32_t MOTOR_POS_100HZ    = 0x0A5;
    static const uint32_t CURRENT_100HZ      = 0x0A6;
    static const uint32_t VOLTAGE_100HZ      = 0x0A7;

};

namespace NetworkConstants
{
    static const uint32_t iCONFIG_SERVER_PORT = 3001;
}

#endif //FLIGHTCOMPUTER_CONSTANTS_H
