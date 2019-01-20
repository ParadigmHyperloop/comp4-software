#include <Ethernet.h>
#include <Sodaq_wdt.h>
#include "Timer.h"
#include <NodeStructs.h>

/* Rear Node
 * Stream incrementing data from node, watchdog reset of no connection
 */
uint8_t nodeNum	 	= 1;
uint8_t nodeState 	= 0;
uint8_t bbbState	= 0;

unsigned char 	mac[] = { 0x02, nodeNum, 0xFF, 0xFF, 0xFF, 0xFF }; //unique mac address
IPAddress 		serverIP(192,168,0,1);
EthernetServer 	server(80); //server port
EthernetClient 	tcpClient;
void 	connectToTCP();
int 	readTCP();

Timer txTimer;
void sendToBBB(void *ignore );
unsigned txNum = 0;

Timer readADCTimer;
void readADCs(void *ignore );
uint8_t ADCBuffer[16] = {65};

// globals
String 			readString;
unsigned long ulStartTime;
unsigned long ulCurrentTime;
unsigned long ticks = 0;
unsigned int  sends = 0;
unsigned char cDataBuffer[256];
char endOfHeaders[] = "\r\n\r\n";
bool bSend = false;
bool bTCPConnected = false;
bool bSendCallback;

void setup(){
	Serial.begin(9600);
	Serial.write("Node 1 Serial started, starting Ethernet next\r\n");

	//Ethernet.init(pin) to configure the CS pin
	Ethernet.init(NW_CS);
	Ethernet.begin(mac, serverIP);

	// Check for Ethernet hardware present
	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
		Serial.println("Wiznet was not found or is not responding");
	}
	else if (Ethernet.linkStatus() == LinkOFF) {
	    Serial.println("Ethernet cable is not connected.");
	}
	else
		Serial.write("Wiznet ready, cable connected");

	// start the server
	server.begin();
	Serial.print("Server started at ");
	Serial.println(Ethernet.localIP());

	// WD flag set at period, WD trigger restart at 2x period
	sodaq_wdt_enable(WDT_PERIOD_8X);
	Serial.write("Watchdog started\r\n");

	// TCP send timer
	txTimer.every(50, sendToBBB, (void*)0);
	Serial.println("txTimer started");
	readADCTimer.every(80, readADCs, (void*)0);
	Serial.println("readAdcTimer started");
}

// main
void loop() {
	inverterOn = readInverter(); // read inverter fn needed

	// if no tcp connection, listen for incoming clients
	if (!tcpClient.connected()) {
		tcpClient = server.available();
		if (tcpClient){
			Serial.println("tcp server available");
			connectToTCP();
		}
	}
	else {
		readTCP();
		txTimer.update();
		readADCTimer.update();
	}

	// If the WDT interrupt has been triggered
/*	if (sodaq_wdt_flag) {
		Serial.write("WDT triggered, starting brakeSequence \n");
		sodaq_wdt_flag = false;
		startBrakeSequence();
	}*/
    ticks++;
    // sodaq_wdt_safe_delay(1000); // delay that pauses WDT while delayed
}

void connectToTCP(){
	Serial.println("new tcp client");
	if (tcpClient.available()) {
		int numRead = tcpClient.read(cDataBuffer, 256);
		Serial.write(cDataBuffer, numRead);
		Serial.println();
		Serial.write("numRead = ");
		Serial.println(numRead);
		if (numRead >= 1){
			sodaq_wdt_reset();
			bTCPConnected = true;
			tcpClient.print(numRead);
		}
	}
}

int readTCP(){
	if (tcpClient.available()) {
		int numRead = tcpClient.read(cDataBuffer, 256);
		Serial.write(cDataBuffer, numRead);
		Serial.println();
		Serial.write("numRead = ");
		Serial.println(numRead);
		if (numRead >= 1){
			sodaq_wdt_reset();
			bTCPConnected = true;
			tcpClient.print(numRead);
		}
		if (cDataBuffer[0] == 'S'){
			sodaq_wdt_reset();
			nodeState = cDataBuffer[6];
		}
	}
}

void sendToBBB(void *ignore){
	Serial.println("sendToBBB triggered");
	if (tcpClient.connected()) {
		sodaq_wdt_reset(); // remove from here when receiving consistantly
		Serial.println("trying to send to tcp client 'ticks'");
		tcpClient.write("txNum = ");
		tcpClient.print(txNum);
		tcpClient.write(" ");
		tcpClient.write("nodeState = ");
		tcpClient.print(nodeState);
		tcpClient.write(" ");
		tcpClient.write(ADCBuffer, 16);
		Serial.write(ADCBuffer, 16);
		tcpClient.println();
		txNum ++;
	}
}

void readADCs(void *ignore){
	for(int i = 0; i < 16; i++){
		if (ADCBuffer[i] == 90)
			ADCBuffer[i] = 65;
		else
			ADCBuffer[i] ++;
	}
}

bool startBrakeSequence(bool fromBBB){
	if (fromBBB){
		Serial.write("Brake sequence started from bbb");
		sodaq_wdt_reset();
		while (inverterOn){
			//waiting for inverted gpio to go low, WDT will trigger if this takes too long
		}
	}
	else {
		sodaq_wdt_reset();
		inverterOn = false; // drive inverter low
	}
	Serial.write("Inverter off");
	activateSolenoidH1();
	sodaq_wdt_safe_delay(10); // 10ms brake delay to engauge
	brakesOn = readBrakes(); // need brakes read fn
	if (brakesOn){
		Serial.write("Brakes on");
		tcpClient.println("Brakes on");
		sodaq_wdt_reset();
		return true;
	}
	else{
		activateSolenoidH2();
		sodaq_wdt_safe_delay(10);
		brakesOn = readBrakes();
		if (brakesOn){
			Serial.write("Brakes on");
			tcpClient.println("Brakes on");
			sodaq_wdt_reset();
			return true;
		}
	}
	// Brake node 2 should kick in in this instance
	Serial.write("Brakes did not work");
	tcpClient.println("Brakes did not work");
	sodaq_wdt_reset();
	return false;
}

bool readInverter(){
	return true;
}

bool readBrakes(){
	return true;
}

void activateSolenoidH1(){
	;
}

void activateSolenoidH2(){
	;
}

void activateSolenoidBleed(){
	;
}
