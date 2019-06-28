#define TACH_INTER 19
#define LEFT_INTER 18
#define RIGHT_INTER 3

char dataBuffer [60];
float micro_to_min = 60000000.0;

volatile double tach_revolutions, left_revolutions,right_revolutions = 0; 
unsigned int total_tach_revolutions, total_ir_revolutions = 0;

unsigned int tach_rpm, left_rpm, right_rpm = 0;
double last_measurment_time, print_time = millis();
float elapsed_time;

void tach_isr(){
  tach_revolutions++;
}

void left_isr(){
  left_revolutions++;
}

void right_isr(){
  right_revolutions++;
}


void setup() {
  Serial.begin(9600);
  Serial.println("starting...");
  attachInterrupt(digitalPinToInterrupt(TACH_INTER), tach_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(LEFT_INTER), left_isr, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_INTER), right_isr, RISING); // Note both are on left inter for load simulation
}

void loop() {

  if (Serial.available() > 0) {
  //if(millis()-print_time > 2000){
    Serial.read();
    detachInterrupt(TACH_INTER); 
    
    elapsed_time = millis()-last_measurment_time;
    float time_period = 60000.0/elapsed_time;
    tach_rpm = time_period*tach_revolutions;
    left_rpm = 60000.0/(elapsed_time)*left_revolutions;
    
    total_tach_revolutions += tach_revolutions;
    total_ir_revolutions += left_revolutions;
    
    sprintf(dataBuffer,"%05u,%05u,%05u,%05u",tach_rpm,left_rpm,total_tach_revolutions,total_ir_revolutions);
    Serial.print(dataBuffer);
    attachInterrupt(digitalPinToInterrupt(TACH_INTER), tach_isr, FALLING);
    
    tach_revolutions = 0;
    left_revolutions = 0;
    last_measurment_time = millis();
    print_time = millis();
    memset(dataBuffer,0,sizeof(dataBuffer));
  }
  
}
