#include <Arduino.h>
#include <Timer.h>

Timer printTimer;
Timer irEqualizeTimer;

const float DISTANCE_PER_SPOKE = 0.1; // circumference of wheel / num of spokes
const uint16_t PRINT_INTERVAL = 200; // print every x milliseconds
const uint8_t LEFT_IR_INT_PIN = 2; // interrupt pin for left IR sensor
const uint8_t RIGHT_IR_INT_PIN = 3; // interrupt pin for right IR sensor
const uint8_t TACH_INT_PIN = 18; //  interrupt pin for optical tachometer
const uint16_t IR_EQUALIZE_DELAY = 50; // run `equalizeIrCount` every x milliseconds

uint32_t spoke_count = 0;
uint32_t left_strip_count = 0;
uint32_t right_strip_count = 0;

bool left_ir_triggered_flag = false;
bool right_ir_triggered_flag = false;

// called on tach interrupt
void tachISR() {
    spoke_count++;
}

// this allows the system to account for if IR on one side misses a strip
void equalizeIrCount(void*) {
    // stop tach interrupts while doing this calculation
    detachInterrupt(TACH_INT_PIN);
    // if both sense a strip
    if (right_ir_triggered_flag && left_ir_triggered_flag) {
        // reset flags
        right_ir_triggered_flag = false;
        left_ir_triggered_flag = false;
    // if only one senses it, make the counts equal to the highest one
    } else if (right_ir_triggered_flag) {
        right_ir_triggered_flag = false;
        left_strip_count = right_strip_count;
    } else if (left_ir_triggered_flag) {
        left_ir_triggered_flag = false;
        right_strip_count = left_strip_count;
    }
    attachInterrupt(digitalPinToInterrupt(TACH_INT_PIN), tachISR, FALLING);
}

// called on IR interrupt
void leftIrISR() {
    left_strip_count++;
    left_ir_triggered_flag = true;
    if (!right_ir_triggered_flag) {
        // start timer to equalize the counts
        irEqualizeTimer.after(IR_EQUALIZE_DELAY, equalizeIrCount, (void*)0);
    }
}

void rightIrISR() {
    right_strip_count++;
    right_ir_triggered_flag = true;
    if (!left_ir_triggered_flag) {
        irEqualizeTimer.after(IR_EQUALIZE_DELAY, equalizeIrCount, (void*)0);
    }
}

// send data to flight computer over serial
void printData(void*) {
    // stop tach interrupts while printing
    detachInterrupt(TACH_INT_PIN);
    Serial.print(spoke_count);
    Serial.print(",");
    if (right_strip_count >= left_strip_count) {
        Serial.println(right_strip_count);
    } else {
        Serial.println(left_strip_count);
    }
    attachInterrupt(digitalPinToInterrupt(TACH_INT_PIN), tachISR, FALLING);
}

void setup() {
    Serial.begin(115200);
    attachInterrupt(digitalPinToInterrupt(TACH_INT_PIN), tachISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(RIGHT_IR_INT_PIN), rightIrISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(LEFT_IR_INT_PIN), leftIrISR, RISING);
    printTimer.every(PRINT_INTERVAL, &printData, (void*)0);
}

void loop() {
    printTimer.update();
    irEqualizeTimer.update();
}
