#include <Arduino.h>
#include <Timer.h>

Timer printTimer;

const uint8_t  NUM_SPOKES = 8;
const uint16_t PRINT_INTERVAL = 100;
const uint8_t  IR_INT_PIN = 2;
const uint8_t  TACH_INT_PIN = 18;

uint16_t spoke_count = 0;
uint16_t ir_strip_count = 0;

void IR_ISR() {
    ir_strip_count++
}

void TACH_ISR() {
    spoke_count++;
}

// send data to flight computer over serial
void printData(void*) {
    // stop tach interrupts while printing
    detachInterrupt(TACH_INT_PIN);
    detachInterrupt(IR_INT_PIN);

    attachInterrupt(digitalPinToInterrupt(TACH_INT_PIN), tachISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(IR_INT_PIN), rightIrISR, FALLING);
}

void setup() {
    Serial.begin(115200);
    attachInterrupt(digitalPinToInterrupt(TACH_INT_PIN), tachISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(IR_INT_PIN), rightIrISR, FALLING);
    printTimer.every(PRINT_INTERVAL, &printData, (void*)0);
}

void loop() {
    printTimer.update();
}
