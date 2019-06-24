/* Arduino Tachometer main for reading fron wheel rpm's */

const unsigned int INTERRUPT_PIN = 2;
const unsigned int RESOLUTION_THRESHOLD = 2;
const float MICRO_TO_MIN = 60000000.0;
const float NUM_SPOKES = 32.0;

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
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), myISR, RISING);
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
    if(millis() - resolution_time > RESOLUTION_THRESHOLD ){
      if(revolutions > 0){
        detachInterrupt(INTERRUPT_PIN2); 
        float rotation_time = micros() - rpm_time;
        rpm = (revolutions * MICRO_TO_MIN)/(rotation_time * NUM_SPOKES);  
        rpm_time = micros();
        revolutions = 0;
        rotation_time = micros();
        resolution_time = millis(); 
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN2), myISR, RISING);
      }
    }
  return;
}
