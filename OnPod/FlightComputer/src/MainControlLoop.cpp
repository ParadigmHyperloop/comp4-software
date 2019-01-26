
#include "FlightComputer/structs.h"

bool checkFlags(PodValues* podVals){
	for(int i = 0 ; i < podVals->flagSize ; i++){
		if(podVals->flagsArray[i] == 1){
			return false;
		}
	}
	return true;
}

bool standbyHold(PodValues* podVals){
	if(checkFlags(podVals)){
		return true;
	}
	return false;
}

bool standbyToArming(PodValues* podVals){

	if(podVals->terminalCommand == terminalArm){
		podVals->terminalCommand = terminalNone;
		return 1;
	}
	return 0;
}

bool armingToArmed(PodValues* podVals){
	if (podVals->breakNodeS == bnArmed){
		return true;
	}
	return false;
}

bool armedToFlight(PodValues* podVals){


	return false;
}

int runControlLoop(PodValues* podVals){

	PodStates podState = podVals->podS;

	switch(podState){

		case standby:{
			if( checkFlags(podVals)){
				if(standbyToArming(podVals)){
					podVals->podS = arming;
				}
				break;
			}
			else{
				podVals->podS = disarm;
			}
			break;
		}
		case arming: {
			if(checkFlags(podVals)){
				if(armingToArmed(podVals)){
					podVals->podS = armed;
				}
				break;
			}
			else{
				podVals->podS = disarm;
			}
			break;
		}
		case armed:{
			if(checkFlags(podVals)){
				if(armedToFlight(podVals)){
					podVals->podS = acceleration;
				}
				break;
			}
			else{
				podVals->podS = disarm;
			}
			break;
		}
		case acceleration:{
			break;
		}
		case coasting: {
			break;
		}
		default:
			break;


	}
return 0;
}
