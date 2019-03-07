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

}




