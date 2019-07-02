/* Board: Arduino Mega
 *
 * Navigation node module for sending the number of spokes counted
 * and the highest count of lines from both IR sensors.
 * This utilizes the tachometer and two IR sensors and will
 * transmit the calculated values serially.
 */

#include <Arduino.h>
#include <Timer.h>

Timer printTimer;

const uint16_t PRINT_INTERVAL = 1000;
const uint8_t IR_INT_PIN = 18;
const uint8_t TACH_INT_PIN = 19;
const uint32_t MICROS_PER_MIN = 60000000;
const uint8_t NUM_SPOKES = 8;

uint8_t data_buffer[40];

uint32_t total_spoke_count = 0;
uint32_t spoke_count = 0;
float tach_rpm = 0;
uint32_t total_strip_count = 0;
uint32_t strip_count = 0;
float ir_rpm = 0;

float elapsed_minutes = 0;
uint64_t elapsed_micros = 0;

void tachISR() {
    spoke_count++;
}

void irISR() {
    strip_count++;
}

void printData() {
    detachInterrupt(TACH_INT_PIN);
    detachInterrupt(IR_INT_PIN);

    Serial.read();
    elapsed_micros = micros() - elapsed_micros;
    elapsed_minutes = (float)elapsed_micros / MICROS_PER_MIN;
    tach_rpm = (float)spoke_count / NUM_SPOKES / elapsed_minutes;
    ir_rpm = strip_count / elapsed_minutes;

    total_spoke_count += spoke_count;
    total_strip_count += strip_count;

    sprintf(
        data_buffer,
        "%08lu,%08lu,%08lu,%08lu",
        (uint32_t)tach_rpm,
        (uint32_t)ir_rpm,
        total_spoke_count,
        total_strip_count
    );
    Serial.print((char*)data_buffer);

    strip_count = 0;
    ir_rpm = 0;
    spoke_count = 0;
    tach_rpm = 0;
    memset(data_buffer, 0, sizeof(data_buffer));

    attachInterrupt(digitalPinToInterrupt(TACH_INT_PIN), tachISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(IR_INT_PIN), irISR, RISING);

    elapsed_micros = micros();
}

void setup() {
    pinMode(53, OUTPUT);
    digitalWrite(53, false);
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(TACH_INT_PIN), tachISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(IR_INT_PIN), irISR, RISING);
    printTimer.every(PRINT_INTERVAL, printData, (void*)0);
}

void loop() {
    if (Serial.available() > 0) {
        digitalWrite(53, true);
        printData();
    }
}
