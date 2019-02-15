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
bool nominalStates(BreakNodeStates bnsState, BreakNodeStates bnsAcceptedStates[], int iAcceptedStatesSize){
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
  TerminalCommands terminalState = pvPodValues->TerminalCommand;
  BreakNodeStates nodeState = pvPodValues->BreakNodeState;
  if ((terminalState == tcTerminalFlight) & (nodeState == bnsFlight))
  {
    pvPodValues->TerminalCommand = tcTerminalNone;
    pvPodValues->MotorState = msDrive;
    return true;
  }
  return false;
}

int runControlLoop(PodValues* podVals)
{
  PodStates podState = podVals->PodState;

  switch (podState)
  {

    case psStandby:
    {
      BreakNodeStates bnAccepted[] = {bnsBooting};
      if (checkFlags(podVals) & nominalStates(podVals->BreakNodeState, bnAccepted, 2))
      {
        if (standbyToArming(podVals))
        {
          podVals->PodState = psArming;
        }
        break;
      }
      else
      {
        podVals->PodState = psDisarm;
      }
      break;
    }

    case psArming:
    {
      BreakNodeStates bnAccepted[] = {bnsArming, bnsArmed};
      if (checkFlags(podVals) & nominalStates(podVals->BreakNodeState, bnAccepted, 2))
      {
        if (armingToArmed(podVals))
        {
          podVals->PodState = psArmed;
        }
        break;
      }
      else
      {
        podVals->PodState = psDisarm;
      }
      break;
    }

    case psArmed:
    {
      BreakNodeStates bnAccepted[] = {bnsArmed};
      if (checkFlags(podVals) & nominalStates(podVals->BreakNodeState, bnAccepted, 1))
      {
        if (armedToFlight(podVals))
        {
          podVals->PodState = psAcceleration;
        }
        break;
      }
      else
      {
        podVals->PodState = psDisarm;
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
