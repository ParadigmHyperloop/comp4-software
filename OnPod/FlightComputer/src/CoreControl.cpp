#include "FlightComputer/structs.h"
#include "FlightComputer/Pod.h"


//template allowableStates


/**Common Check thats must be done
 * before all state checks
 */
void commonChecks(Pod podValues){
	// Check flags
	// Check heartbeat flags
}

void bootingTransitions(Pod podValues)
{
	// If all nodes are in standby go to standby
	podValues.setPodState(psStandby);
}

void standyTransitions(Pod podValues)
{
	// If terminal command is set to arm transition to arming

	// Nodes stay in standby

	// Abort transitions coverred by commonChecks
	podValues.setPodState(psArming);
}

void armingTransitions(Pod podValues)
{
	// If all brake nodes are in an armed state then go to armed
	// Abort transitions coverred by commonChecks
	podValues.setPodState(psArmed);
}

void armedTransitions(Pod podValues)
{

	// check if all brake nodes are still armed

	// Terminal command to flight
	podValues.setPodState(psAcceleration);

}

void accelerationTransitions(Pod podValues)
{

	// Check all nodes in their correct state

	// Acceleration watchdog

	// Check Navigation module state

	podValues.setPodState(psCoasting);
}

void coastingTranstions(Pod podValues)
{
	// go to braking
	podValues.setPodState(psBraking);
}

void brakingTransitons(Pod podValues)
{
	// we have to brake for XX seconds

	// The navs have to say we are at rest

	// Or we are given a command to override the state --  this might be done through the commander
	podValues.setPodState(psDisarm);
}

void disarmTransitions(Pod podValues)
{

	// Check that the state of all nodes are in retrieval

	// check the BMS is all good

	// Go to retrieval
	podValues.setPodState(psRetrieval);
}

void retrievalTransitions(Pod podValues)
{

	//Hold retrieval conditions
	return;
}



void coreControlLoop(Pod podValues){

	while(1)
	{
		commonChecks(podValues);
		PodStates ePodState = podValues.getPodState();
		switch(ePodState)
		{
		case psBooting:
		{
			bootingTransitions(podValues);
			break;
		}
		case psStandby:
		{
			standyTransitions(podValues);
			break;
		}
		case psArming:
		{
			armingTransitions(podValues);
			break;
		}

		}
		case psArmed:
		{
			armedTransitions(podValues);
			break;
		}
		case psAcceleration:
		{
			accelerationTransitions(podValues);
			break;
		}
		case psCoasting:
		{
			coastingTranstions(podValues);
			break;
		}
		case psBraking:
		{
			brakingTransitons(podValues);
			break;
		}
		case psDisarm:
		{
			disarmTransitions(podValues);
			break;
		}
		case psRetrieval:
		{
			retrievalTransitions(podValues);
			break;
		}
		case psEmergency:
		{
			break;
		}
	}

}





