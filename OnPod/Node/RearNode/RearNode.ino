#include <Ethernet.h>
#include <Sodaq_wdt.h>
#include "Timer.h"
#include <NodeStructs.h>

/* Rear Node
 * Stream incrementing data from node, watchdog reset of no connection
 */
uint8_t nodeNum	 	= REAR;
RearPkg data;
unsigned bbbTXDelayMS = MIN; // MIN = 100ms
unsigned readADCDelay = 80;  // ms

// Ethernet setup for tcp
unsigned char 	mac[] = { 0x03, nodeNum, 0xFF, 0xFF, 0xFF, 0xFF }; //unique mac address
IPAddress 		serverIP(192,168,0,1);
EthernetServer 	server(80); //server port
EthernetClient 	tcpClient;

// TX timer for sending over Ethernet
Timer txTimer;
void sendToBBB(void *ignore );
unsigned txNum = 0;

// Timer for how often to read ADC's
Timer readADCTimer;
void readADCs(void *ignore );

// globals, can be cleaned up / moved to NodeStruct.h
String 			readString;
unsigned long ulStartTime;
unsigned long ulCurrentTime;
unsigned long ticks = 0;
unsigned int  sends = 0;
unsigned char cDataBuffer[256];

bool bSend = false;
bool bTCPConnected = false;
bool bSendCallback;

void setup(){
	Serial.begin(9600);
	Serial.write("Rear Node Serial started, starting Ethernet next\r\n");

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
	txTimer.every(bbbTXDelayMS, sendToBBB, (void*)0);
	Serial.println("txTimer started");
	readADCTimer.every(readADCDelay, readADCs, (void*)0);
	Serial.println("readAdcTimer started");
}

// main
void loop() {
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

// parse tcp message and update data's bbbState value
void readTCP(){
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
		if (cDataBuffer[0] == 'S'){ // test msg of hex: 53 74 61 74 65 20 31 20 66 72 6f 6d 20 42 42 42
			sodaq_wdt_reset();
			data.bbbState = cDataBuffer[6]; // should be 1
		}
	}
}

void sendToBBB(void *ignore){
	Serial.println("sendToBBB triggered");
	if (tcpClient.connected()) {
		sodaq_wdt_reset(); // remove from here when receiving consistently
		Serial.println("trying to send to tcp client 'ticks'");
		tcpClient.write("txNum = ");
		tcpClient.print(txNum);
		tcpClient.write(" ");
		tcpClient.write("confirming bbbState ");
		tcpClient.print(data.bbbState); // send what noode thinks bbbState is
		tcpClient.write(" ");
		tcpClient.println();
	}
}

// implement reading ADC values over spi
void readADCs(void *ignore){
	;
}
