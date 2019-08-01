/* Board: Arduino Mega
 *
 * Navigation node module for sending the number of spokes counted
 * and the highest count of lines from both IR sensors.
 * This utilizes the tachometer and two IR sensors and will
 * transmit the calculated values serially.
 */

#include <Arduino.h>
#include <Timer.h>
#include <math.h>

#include "sensors/atmo_transducer_HSC60P.h"

const uint8_t LEFT_IR_INT_PIN = 18;   // interrupt pin for left IR sensor
const uint8_t RIGHT_IR_INT_PIN = 2;    // interrupt pin for right IR sensor
const uint32_t DISTANCE_BEWTEEN_STRIPS = 3048; // cm between strips
const uint32_t MILLIS_PER_SEC = 1000;

char data_buffer[20];
bool strip_detected = false;
uint32_t strip_time = 0;
uint32_t last_strip_time = 0;
uint32_t velocity = 0;
uint8_t pressure = 0;

// timer to prevent double detection of strip pairs
Timer cooldownTimer;
bool cooldown_active = false;
const uint16_t COOLDOWN_PERIOD = 10;

HSC60P pressureTransducer(0x38);

void expireCooldown(void*) {
    cooldown_active = false;
}

// called on IR interrupt
void irISR() {
    if (!cooldown_active) {
        strip_detected = true;
        cooldown_active = true;
        cooldownTimer.after(COOLDOWN_PERIOD, expireCooldown, (void*)0);
        strip_time = millis();
    }
}

// send data to flight computer over serial
void printData() {
    Serial.read();
    if (strip_detected) {
        velocity = DISTANCE_BEWTEEN_STRIPS * MILLIS_PER_SEC / (strip_time-last_strip_time);
        sprintf(data_buffer, "aa,%05lu,%02u", strip_detected, velocity, pressure);
        strip_detected = false;
    }
    else {
        sprintf(data_buffer, "%02u,%05lu,%02u", strip_detected, velocity, pressure);
    }
    Serial.print(data_buffer);
    last_strip_time = strip_time;
    memset(data_buffer, 0, sizeof(data_buffer));
}

void setup() {
    Serial.begin(9600);
    pinMode(RIGHT_IR_INT_PIN, INPUT);
    pinMode(LEFT_IR_INT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(RIGHT_IR_INT_PIN), irISR, RISING);
    attachInterrupt(digitalPinToInterrupt(LEFT_IR_INT_PIN), irISR, RISING);
}

void loop() {
    // if request for data from the flight computer
    if (Serial.available() > 0) {
        printData();
    }
    cooldownTimer.update();
}
