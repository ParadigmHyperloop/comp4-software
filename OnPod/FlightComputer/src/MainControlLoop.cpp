#include "FlightComputer/structs.h"

/**
 * The flags array should be all zeros if all systems are nominal.
 * Each index represents a different subsystem.
 */
bool checkFlags(PodValues* pvPodValues)
{
  for (int i = 0; i < pvPodValues->iFlagsArraySize; i++)
  {
    if (pvPodValues->cFlagsArray[i] == 1)
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
bool nominalStates(eBreakNodeStates bnsState, eBreakNodeStates bnsAcceptedStates[], int iAcceptedStatesSize){
  for (int i = 0; i < iAcceptedStatesSize; i++)
  {
    if (bnsAcceptedStates[i] == bnsState)
    {
      return true;
    }
  }
  return false;
}

bool standbyToArming(PodValues* pvPodValues)
{
  if (pvPodValues->TerminalCommand == tcTerminalArm)
  {
    pvPodValues->TerminalCommand = tcTerminalNone;
    return true;
  }
  return false;
}

bool armingToArmed(PodValues* pvPodValues)
{
  if (pvPodValues->BreakNodeState == bnsArmed)
  {
    return true;
  }
  return false;
}

bool armedToFlight(PodValues* pvPodValues)
{
  eTerminalCommands terminalState = pvPodValues->TerminalCommand;
  eBreakNodeStates nodeState = pvPodValues->BreakNodeState;
  if ((terminalState == tcTerminalFlight) & (nodeState == bnsFlight))
  {
    pvPodValues->TerminalCommand = tcTerminalNone;
    pvPodValues->MotorState = msDrive;
    return true;
  }
  return false;
}

int runControlLoop(PodValues* pvPodValues)
{
  ePodStates podState = pvPodValues->PodState;
  switch (podState)
  {

    case psStandby:
    {
      eBreakNodeStates bnAccepted[] = {bnsBooting};
      if (checkFlags(pvPodValues) & nominalStates(pvPodValues->BreakNodeState, bnAccepted, 2))
      {
        if (standbyToArming(pvPodValues))
        {
          pvPodValues->PodState = psArming;
        }
        break;
      }
      else
      {
        pvPodValues->PodState = psDisarm;
      }
      break;
    }

    case psArming:
    {
      eBreakNodeStates bnAccepted[] = {bnsArming, bnsArmed};
      if (checkFlags(pvPodValues) & nominalStates(pvPodValues->BreakNodeState, bnAccepted, 2))
      {
        if (armingToArmed(pvPodValues))
        {
          pvPodValues->PodState = psArmed;
        }
        break;
      }
      else
      {
        pvPodValues->PodState = psDisarm;
      }
      break;
    }

    case psArmed:
    {
      eBreakNodeStates bnAccepted[] = {bnsArmed};
      if (checkFlags(pvPodValues) & nominalStates(pvPodValues->BreakNodeState, bnAccepted, 1))
      {
        if (armedToFlight(pvPodValues))
        {
          pvPodValues->PodState = psAcceleration;
        }
        break;
      }
      else
      {
        pvPodValues->PodState = psDisarm;
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
