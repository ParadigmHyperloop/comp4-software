
#include "FlightComputer/structs.h"

/**
 * The flags array should be all zeros if all systems are nominal.
 * Each index represents a different subsystem.
 */
bool checkFlags(PodValues* podVals) {
  for (int i = 0; i < podVals->flagSize; i++) {
    if (podVals->flagsArray[i] == 1) {
      return false;
    }
  }
  return true;
}

/**
 * Checks that the current state of the break node is within
 * one of its expected states.
 */
bool nominalStates(BreakNodeStates bnState, BreakNodeStates bnAccepted[],
                   int bnAcceptedSize) {
  for (int i = 0; i < bnAcceptedSize; i++) {
    if (bnAccepted[i] == bnState) {
      return true;
    }
  }
  return false;
}

bool standbyToArming(PodValues* podVals) {
  if (podVals->terminalCommand == terminalArm) {
    podVals->terminalCommand = terminalNone;
    return true;
  }
  return false;
}

bool armingToArmed(PodValues* podVals) {
  if (podVals->breakNodeS == bnArmed) {
    return true;
  }
  return false;
}

bool armedToFlight(PodValues* podVals) {
  TerminalCommands terminalState = podVals->terminalCommand;
  BreakNodeStates nodeState = podVals->breakNodeS;
  if ((terminalState == terminalFlight) & (nodeState == bnFlight)) {
    podVals->terminalCommand = terminalNone;
    podVals->motorS = drive;
    return true;
  }
  return false;
}

int runControlLoop(PodValues* podVals) {
  PodStates podState = podVals->podS;

  switch (podState) {
    case standby: {
      BreakNodeStates bnAccepted[] = {bnBooting, bnBooting};

      if (checkFlags(podVals) &
          nominalStates(podVals->breakNodeS, bnAccepted, 2)) {
        if (standbyToArming(podVals)) {
          podVals->podS = arming;
        }
        break;
      } else {
        podVals->podS = disarm;
      }
      break;
    }
    case arming: {
      BreakNodeStates bnAccepted[] = {bnArming, bnArmed};

      if (checkFlags(podVals) &
          nominalStates(podVals->breakNodeS, bnAccepted, 2)) {
        if (armingToArmed(podVals)) {
          podVals->podS = armed;
        }
        break;
      } else {
        podVals->podS = disarm;
      }
      break;
    }
    case armed: {
      BreakNodeStates bnAccepted[] = {bnArmed};

      if (checkFlags(podVals) &
          nominalStates(podVals->breakNodeS, bnAccepted, 1)) {
        if (armedToFlight(podVals)) {
          podVals->podS = acceleration;
        }
        break;
      } else {
        podVals->podS = disarm;
      }
      break;
    }
    case acceleration: {
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
