#include "FlightComputer/structs.h"
#include "FlightComputer/Pod.h"
#include "FlightComputer/CoreControl.h"


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
	BrakeNodeStates eBrakeNodeState = podValues.getBrakeNodeState();
	if(eBrakeNodeState == bnsStandby)
	{
		podValues.setPodState(psStandby, "All nodes in Standby");
	}
}

void standyTransitions(Pod podValues)
{
	// If terminal command is set to arm transition to arming

	// Nodes stay in standby

	// Abort transitions coverred by commonChecks
	podValues.setPodState(psArming, "Automatic");
}

void armingTransitions(Pod podValues)
{
	// If all brake nodes are in an armed state then go to armed
	// Abort transitions coverred by commonChecks
	BrakeNodeStates eBrakeNodeState = podValues.getBrakeNodeState();
	if(eBrakeNodeState == bnsArmed)
	{
	podValues.setPodState(psArmed, "All nodes Armed");
	}
}

void armedTransitions(Pod podValues)
{

	// check if all brake nodes are still armed

	// Terminal command to flight
	podValues.setPodState(psPreFlight,"Automatic");

}

void preflightTransitions(Pod podValues)
{
	// Or we are given a command to override the state --  this might be done through the commander
	BrakeNodeStates eBrakeNodeState = podValues.getBrakeNodeState();
	if(eBrakeNodeState == bnsFlight)
	{
	podValues.setPodState(psAcceleration,"Brake Node in Flight State");
	}
}

void accelerationTransitions(Pod podValues)
{

	// Check all nodes in their correct state

	// Acceleration watchdog

	// Check Navigation module state

	podValues.setPodState(psCoasting,"Automatic");
}

void coastingTranstions(Pod podValues)
{
	// go to braking
	podValues.setPodState(psBraking,"Automatic");
}

void brakingTransitons(Pod podValues)
{
	// we have to brake for XX seconds

	// The navs have to say we are at rest

	// Or we are given a command to override the state --  this might be done through the commander
	BrakeNodeStates eBrakeNodeState = podValues.getBrakeNodeState();
	if(eBrakeNodeState == bnsBraking)
	{
	podValues.setPodState(psDisarming,"Brake Node in Braking");
	}
}

void disarmTransitions(Pod podValues)
{

	// Check that the state of all nodes are in retrieval

	// check the BMS is all good

	// Go to retrieval
	BrakeNodeStates eBrakeNodeState = podValues.getBrakeNodeState();
	if(eBrakeNodeState == bnsRetrieval)
	{
	podValues.setPodState(psRetrieval,"Brake Node in Retrieval");
	}
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
		case psArmed:
		{
			armedTransitions(podValues);
			break;
		}
		case psPreFlight:
		{
			preflightTransitions(podValues);
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
		case psDisarming:
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
}






