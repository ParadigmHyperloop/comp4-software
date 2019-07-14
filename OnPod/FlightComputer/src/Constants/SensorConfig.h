#ifndef FLIGHTCOMPUTER_SENSORCONFIG_H
#define FLIGHTCOMPUTER_SENSORCONFIG_H

// Count



// const ants
/* 
VACUUM 0
ATMOSPHERE 15
CLOSED 0
OPEN 1
*/
struct CONSTANTS
{
    static const int32_t VACUUM = 0;
    static const int32_t ATMOSPHERE = 15;

    static const int32_t CLOSED = 0;
    static const int32_t OPEN = 1;
};

// Connection Status Flags
struct CONNECTION_FLAGS
{
    static const int32_t TOTAL_CONNECTION_COUNT = 6;  // update if you add
    static const int32_t BRAKE_NODE_HEARTBEAT_INDEX = 0;
    static const int32_t LVDC_NODE_HEARTBEAT_INDEX = 1; 
    static const int32_t BMS_HEARTBEAT_INDEX = 2;
    static const int32_t INTERFACE_HEARTBEAT_INDEX = 3;
    static const int32_t ENCLOSURE_HEARTBEAT_INDEX = 4;
    static const int32_t NAVIGATION_HEARTBEAT_INDEX = 5;
};

// Inverter Sensor Flag Indices
 struct INVERTER_FLAGS
{
    static const int32_t INVERTER_SENSOR_COUNT = 5;  // update if you add
    static const int32_t MOTOR_TEMPERATURE_INDEX = 0;
    static const int32_t BUS_VOLTAGE_INDEX = 1;
    static const int32_t GATE_DRIVER_TEMP_INDEX = 2;
    static const int32_t CONTROL_BOARD_TEMP_INDEX = 3;
    static const int32_t MAX_IGBT_INDEX = 4;
};

// BMS Sensor Flag Indices
 struct BMS_FLAGS
{
    static const int32_t BMS_SENSOR_COUNT = 9;  // update if you add
    static const int32_t HV_PACK_VOLTAGE_INDEX = 0;
    static const int32_t HV_MAX_CELL_TEMP_INDEX = 1;
    static const int32_t HV_SOC_INDEX = 2;

    static const int32_t LV1_PACK_VOLTAGE_INDEX = 3;
    static const int32_t LV1_CELL_TEMP_INDEX = 4;
    static const int32_t LV1_SOC_INDEX = 5;

    static const int32_t LV2_PACK_VOLTAGE_INDEX = 6;
    static const int32_t LV2_CELL_TEMP_INDEX = 7;
    static const int32_t LV2_SOC_INDEX = 8;
};

// Node Sensor Flag indices
 struct NODE_FLAGS
{
    static const int32_t NODE_SENSOR_COUNT = 10; // update if you add
    static const int32_t LP1_INDEX = 0;
    static const int32_t LP2_INDEX = 1;
    static const int32_t LP3_INDEX = 2;
    static const int32_t LP4_INDEX = 3;
    static const int32_t HP_INDEX = 4;
    static const int32_t HP_TEMP_INDEX = 5;
    static const int32_t ENCLOSURE_PRESSURE_INDEX = 6;
    static const int32_t ENCLOSURE_TEMPERATURE_INDEX = 7;
    static const int32_t COOLING_PRESSURE_INDEX = 8;
    static const int32_t COOLING_TEMPERATURE_INDEX = 9;
};

// Solenoid Status indices
 struct SOLENOID_INDEX
{
    static const int32_t SOL1 = 0;  // update if you add
    static const int32_t SOL2 = 1;
    static const int32_t SOL3 = 2;
    static const int32_t SOL4 = 3;
};

// Enclosure
 struct ENCLOSURE_LIMITS
{
    static const int32_t ENCLOSURE_TEMP_MIN = 20;
    static const int32_t ENCLOSURE_TEMP_MAX = 60; //todo validate

    static const int32_t COOLING_ENGAGED_MIN = 20; //TODO validate
    static const int32_t COOLING_ENGAGED_MAX = 35;

    static const int32_t COOLING_THRESHOLD = 5;
};


// Inverter
 struct INVERTER_LIMITS
{
    static const int32_t MOTOR_UNARMED_TEMP_MIN = 10;
    static const int32_t MOTOR_UNARMED_TEMP_MAX = 50; //TODO

    static const int32_t MOTOR_ARMED_TEMP_MIN = 10;
    static const int32_t zMOTOR_ARMED_TEMP_MAX = 400; //TODO

    static const int32_t COMPONENT_TEMP_MIN = 10;
    static const int32_t COMPONENT_TEMP_MAX = 400; //TODO
};

// Pneumatics
 struct PNEUMATICS_LIMITs
{
    static const int32_t PNEUMATICS_THRESHOLD = 3; // Margin of acceptable error on ideal value

    static const int32_t LOWPRESSURE_UNARMED_MIN = 0;
    static const int32_t LOWPRESSURE_UNARMED_MAX = 18;
    static const int32_t LOWPRESSURE_ENGAGED_MIN = 215;
    static const int32_t LOWPRESSURE_ENGAGED_MAX = 255;

    static const int32_t HIGHPRESSURE_ARMED_MAX = 1725;
    static const int32_t HIGHPRESSURE_ARMED_MIN = 1550;

    static const int32_t PRESSURE_TEMP_MIN = 0;
    static const int32_t PRESSURE_TEMP_MAX = 60; //C

    static const int32_t COOLING_TEMP_MIN = 10;
    static const int32_t COOLING_TEMP_MAX = 30;
};

float ATMOSPHERE_THRESHOLD = 5.0f;


#endif //FLIGHTCOMPUTER_SENSORCONFIG_H
