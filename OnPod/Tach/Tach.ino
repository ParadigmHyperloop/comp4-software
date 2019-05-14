float micro_to_min = 60000000.0;
float spokes = 32;

float revolutions = 0;
unsigned long rpm_time = micros();
unsigned long print_time = millis();
unsigned long resolution_time = millis();

float rpm = 0;
float new_rpm = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  Serial.println("starting...");
  attachInterrupt(digitalPinToInterrupt(2), myISR, RISING);
}

// the loop function runs over and over again forever
void loop() {
  if(millis()-print_time > 1000){
    Serial.println(rpm);
    print_time = millis();
  }
}

void myISR(){
  revolutions++;
    if(millis() - resolution_time > 500 ){
      if(revolutions>0){
        detachInterrupt(2); 
        float rotation_time = micros() - rpm_time;
        rpm = (revolutions*((float)60000000.0))/(rotation_time*((float)32.0));  
        rpm_time = micros();
        revolutions = 0;
        rotation_time = micros();
        resolution_time = millis(); 
        attachInterrupt(digitalPinToInterrupt(2), myISR, RISING);
      }
    }
  return;
  }
