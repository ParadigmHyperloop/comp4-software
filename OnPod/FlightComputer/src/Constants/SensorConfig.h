#ifndef FLIGHTCOMPUTER_SENSORCONFIG_H
#define FLIGHTCOMPUTER_SENSORCONFIG_H

// Count

#define TOTAL_CONNECTION_COUNT 10

// Constants

#define VACUUM 0
#define ATMOSPHERE 15
#define CLOSED 0
#define OPEN 1

// Inverter Sensor Flag Indices
#define INVERTER_SENSOR_COUNT 2
#define MOTOR_TEMPERATURE_INDEX 0
#define BUS_VOLTAGE_INDEX 1

// BMS Sensor Flag Indices
#define BMS_SENSOR_COUNT 3
#define PACK_VOLTAGE_INDEX 0
#define MAX_CELL_TEMP_INDEX 1
#define SOC_INDEX 2

// Node Sensor Flag indices
#define NODE_SENSOR_COUNT 10
#define LP1_INDEX 0
#define LP2_INDEX 1
#define LP3_INDEX 2
#define HP_INDEX  3
#define HP_TEMP_INDEX 4

// Solenoid Status indices
#define SOL1_INDEX 0
#define SOL2_INDEX 1
#define SOL3_INDEX 2
#define SOL4_INDEX 3


// Pneumatics

#define PNEUMATICS_THRESHOLD 10.0 // Margin of acceptable error on ideal value

#define LOWPRESSURE_UNARMED_MIN 0.0
#define LOWPRESSURE_UNARMED_MAX 20
#define LOWPRESSURE_ENGAGED 250.0

#define HIGHPRESSURE_ARMED_MAX 1750
#define HIGHPRESSURE_ARMED_MIN 1500

#define PRESSURE_TEMP_MIN 55
#define PRESSURE_TEMP_MAX 30




//


#define ATMOSPHERE_THRESHOLD 5.0f


#endif //FLIGHTCOMPUTER_SENSORCONFIG_H
