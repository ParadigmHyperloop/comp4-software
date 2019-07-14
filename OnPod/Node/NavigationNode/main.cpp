/* Board: Arduino Mega
 *
 * Navigation node module for sending the number of spokes counted
 * and the highest count of lines from both IR sensors.
 * This utilizes the tachometer and two IR sensors and will
 * transmit the calculated values serially.
 */

#include <Arduino.h>
#include <Timer.h>

const uint8_t LEFT_IR_INT_PIN = 18;   // interrupt pin for left IR sensor
const uint8_t RIGHT_IR_INT_PIN = 2;    // interrupt pin for right IR sensor
const uint32_t DISTANCE_BEWTEEN_STRIPS = 3048; // cm between strips
const uint32_t MILLIS_PER_SEC = 1000;

char data_buffer[30];
bool strip_detected = false;
uint32_t strip_time = 0;
uint32_t last_strip_time = 0;
uint32_t velocity = 0;

// called on IR interrupt
void irISR() {
    strip_detected = true;
    strip_time = millis();
}

// send data to flight computer over serial
void printData() {
    Serial.read();
    if (strip_detected) {
        velocity = DISTANCE_BEWTEEN_STRIPS * MILLIS_PER_SEC / (strip_time-last_strip_time);
        strip_detected = false;
    }
    sprintf(data_buffer, "%u,%05lu", strip_detected, velocity);
    Serial.print(data_buffer);
    last_strip_time = strip_time;
    memset(data_buffer, 0, sizeof(data_buffer));
}

void setup() {
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(RIGHT_IR_INT_PIN), irISR, RISING);
    attachInterrupt(digitalPinToInterrupt(LEFT_IR_INT_PIN), irISR, RISING);
}

void loop() {
    // if request for data from the flight computer
    if (Serial.available() > 0) {
        printData();
    }
}
