#include "FlightComputer/structs.h"
#include "FlightComputer/MemoryAccess.h"


//template allowableStates


/**Common Check thats must be done
 * before all state checks
 */
void commonChecks(MemoryAccess podValues){
	// Check flags
	// Check heartbeat flags
}

void standyTransitions(MemoryAccess podValues){
	// If terminal command is set to arm transition to arming

	// Nodes stay in standby

	// Abort transitions coverred by commonChecks
}

void armingTransitions(){
	// If all brake nodes are in an armed state then go to armed
	// Abort transitions coverred by commonChecks
}

void armedTransitions(){

	// check if all brake nodes are still armed

	// Terminal command to flight

}

void accelerationTransitions(){

	// Check all nodes in their correct state

	// Acceleration watchdog

	// Check Navigation module state

	//
}

void coastingTranstions(){
	// go to braking
}

void brakingTransitons(){
	// we have to brake for XX seconds

	// The navs have to say we are at rest

	// Or we are given a command to override the state --  this might be done through the commander
}

void disarmTransitions(){

	// Check that the state of all nodes are in retrieval

	// check the BMS is all good

	// Go to retrieval
}

void retrievalTransitions(){

	//Hold retrieval conditions
}









