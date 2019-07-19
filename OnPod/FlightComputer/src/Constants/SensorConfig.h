#ifndef FLIGHTCOMPUTER_SENSORCONFIG_H
#define FLIGHTCOMPUTER_SENSORCONFIG_H

// Count

// Connection Status Flags

// Enclosure
 namespace ENCLOSURE_LIMITS
{
    static int32_t ENCLOSURE_TEMP_MIN = 20;
    static int32_t ENCLOSURE_TEMP_MAX = 60; //todo validate

    static int32_t COOLING_ENGAGED_MIN = 20; //TODO validate
    static int32_t COOLING_ENGAGED_MAX = 35;

    static int32_t COOLING_THRESHOLD = 5;

    static int32_t PRESSURE_MIN = 10;
    static int32_t PRESSURE_MAX = 19;

    static float ATMOSPHERE_THRESHOLD = 5.0f;
};


// Inverter
 namespace INVERTER_LIMITS
{
    static int32_t MOTOR_UNARMED_TEMP_MIN = 10;
    static int32_t MOTOR_UNARMED_TEMP_MAX = 50;

    static int32_t MOTOR_ARMED_TEMP_MIN = 10;
    static int32_t MOTOR_ARMED_TEMP_MAX = 150;

    static int32_t COMPONENT_TEMP_MIN = 10;
    static int32_t COMPONENT_TEMP_MAX = 150;
};

// High Voltage
namespace HV_LIMITS
{
    static int32_t VOLTAGE_ABSOLUTE_MIN = 716;
    static int32_t VOLTAGE_ARMED_MIN = 777;
    static int32_t VOLTAGE_MAX = 807;

    static int32_t CELL_TEMP_MIN = 10;
    static int32_t CELL_TEMP_MAX = 100;

    static int32_t SOC_ABSOLUTE_MIN = 20;
    static int32_t SOC_ARMED_MIN = 75;
    static int32_t SOC_MAX = 100;
}

// High Voltage
namespace LV_LIMITS
{
    static float VOLTAGE_ABSOLUTE_MIN = 28.6;  //todo
    static float VOLTAGE_MAX = 34;

    static int32_t CELL_TEMP_MIN = 10;
    static int32_t CELL_TEMP_MAX = 70;

    static int32_t SOC_ABSOLUTE_MIN = 25;
    static int32_t SOC_ARMED_MIN = 75;
    static int32_t SOC_MAX = 100;
}



// Pneumatics
 namespace PNEUMATICS_LIMITS
{
    static int32_t PNEUMATICS_THRESHOLD = 3; // Margin of acceptable error on ideal value

    static int32_t LOWPRESSURE_UNARMED_MIN = 0;
    static int32_t LOWPRESSURE_UNARMED_MAX = 20;
    static int32_t LOWPRESSURE_ENGAGED_MIN = 215;
    static int32_t LOWPRESSURE_ENGAGED_MAX = 255;

    static int32_t HIGHPRESSURE_ARMED_MAX = 1725;
    static int32_t HIGHPRESSURE_ARMED_MIN = 1550;

    static int32_t PRESSURE_TEMP_MIN = 0;
    static int32_t PRESSURE_TEMP_MAX = 60; //C

    static int32_t COOLING_TEMP_MIN = 10;
    static int32_t COOLING_TEMP_MAX = 30;
};

#endif //FLIGHTCOMPUTER_SENSORCONFIG_H
