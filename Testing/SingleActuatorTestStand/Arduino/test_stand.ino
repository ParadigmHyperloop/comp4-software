void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
}

bool solenoid_driven;

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    Serial.println(command);
    if (command - '0' == 0 || command - '0' == 1) {
      solenoid_driven = command - '0';
      digitalWrite(LED_BUILTIN, solenoid_driven);
    }
  }
  float tank_pressure = random(1, 1000);
  float piston_pressure = random(1, 1000);
  float temperature_1 = random(1, 1000);
  float temperature_2 = random(1, 1000);
  float load = random(1, 1000);
  Serial.print(millis(), 1);
  Serial.print(",");
  Serial.print(tank_pressure, 2);
  Serial.print(",");
  Serial.print(piston_pressure, 2);
  Serial.print(",");
  Serial.print(temperature_1, 2);
  Serial.print(",");
  Serial.print(temperature_2, 2);
  Serial.print(",");
  Serial.print(load, 2);
  Serial.print(",");
  Serial.print(solenoid_driven);
  Serial.println(",");
 // delay(50);
}
