#include "FlightComputer/structs.h"
#include "FlightComputer/MemoryAccess.h"


/**
 * The flags array should be all zeros if all systems are nominal.
 * Each index represents a different subsystem.
 */
bool checkFlags(unsigned char cFlagsArray[], int iSize)
{
  for (int i = 0; i < iSize; i++)
  {
    if (cFlagsArray[i] == 1)
    {
      return false;
    }
  }
  return true;
}

/**
 * Checks that the current state of the break node is within
 * one of its expected states.
 */
bool nominalStates(fc::brakeNodeData::breakNodeState bnsState, fc::brakeNodeData::breakNodeState bnsAcceptedStates[], int iAcceptedStatesSize){
  for (int i = 0; i < iAcceptedStatesSize; i++)
  {
    if (bnsAcceptedStates[i] == bnsState)
    {
      return true;
    }
  }
  return false;
}

bool standbyToArming(MemoryAccess* Pod)
{
  if (Pod->getTerminalCommand() == tcTerminalArm)
  {
    if(Pod->setTerminalCommand(tcTerminalNone))
    {
    	return true;
    }
    else
    {
    	//TODO THROW ERROR??
    	return false;
    }
  }
  return false;
}

bool armingToArmed(MemoryAccess* Pod)
{
  if (Pod->getBrakeNodeState() == fc::brakeNodeData::bnsArmed)
  {
    return true;
  }
  return false;
}

bool armedToFlight(MemoryAccess* Pod)
{
  eTerminalCommands terminalState = Pod->getTerminalCommand();
  fc::brakeNodeData::breakNodeState nodeState = Pod->getBrakeNodeState();
  if ((terminalState == tcTerminalFlight) & (nodeState == fc::brakeNodeData::bnsFlight))
  {
    Pod->setTerminalCommand(tcTerminalNone);
    Pod->setMotorState(msDrive);
    return true;
  }
  return false;
}

int runControlLoop(MemoryAccess Pod)
{
  ePodStates podState = Pod.getPodState();
  switch (podState)
  {

    case psStandby:
    {
    	fc::brakeNodeData::breakNodeState bnAccepted[] = {fc::brakeNodeData::bnsStandby};
      if (checkFlags(Pod.getFlagsArray(), Pod.getFlagsArraySize()) & nominalStates(Pod.getBrakeNodeState(), bnAccepted, 1))
      {
        if (standbyToArming(&Pod))
        {
          Pod.setPodState(psArming);
        }
        break;
      }
      else
      {
        Pod.setPodState(psDisarm);
      }
      break;
    }

    case psArming:
    {
    	fc::brakeNodeData::breakNodeState bnAccepted[] = {fc::brakeNodeData::bnsArming, fc::brakeNodeData::bnsArmed};
      if (checkFlags(Pod.getFlagsArray(), Pod.getFlagsArraySize()) & nominalStates(Pod.getBrakeNodeState(), bnAccepted, 2))
      {
        if (armingToArmed(&Pod))
        {
          Pod.setPodState(psArmed);
        }
        break;
      }
      else
      {
        Pod.setPodState(psDisarm);
      }
      break;
    }

    case psArmed:
    {
      fc::brakeNodeData::breakNodeState bnAccepted[] = {fc::brakeNodeData::bnsArmed};
      if (checkFlags(Pod.getFlagsArray(), Pod.getFlagsArraySize()) & nominalStates(Pod.getBrakeNodeState(), bnAccepted, 1))
      {
        if (armedToFlight(&Pod))
        {
          Pod.setPodState(psAcceleration);
        }
        break;
      }
      else
      {
        Pod.setPodState(psDisarm);
      }
      break;
    }

    case psAcceleration:
    {
      break;
    }

    case psCoasting: {
      break;
    }

    default:
      break;
  }
  return 0;
}
