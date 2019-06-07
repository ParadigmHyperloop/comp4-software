#include <Arduino.h>

void setup() {
    Serial.begin(9600);
}

void loop() {
    float cell_voltage = analogRead(5);
    float cell_current_low = analogRead(3);
    float cell_current_high = analogRead(4);
    float temp_1 = analogRead(2);
    float temp_2 = analogRead(0);
    float temp_3 = analogRead(1);
    Serial.print(millis(), 1);
    Serial.print(",");
    Serial.print(cell_voltage, 2);
    Serial.print(",");
    Serial.print(cell_current_low, 2);
    Serial.print(",");
    Serial.print(cell_current_high, 2);
    Serial.print(",");
    Serial.print(temp_1, 2);
    Serial.print(",");
    Serial.print(temp_2, 2);
    Serial.print(",");
    Serial.print(temp_3, 2);
    Serial.println(",");
}
