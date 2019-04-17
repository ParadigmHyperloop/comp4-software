# Node Simulator 

The motivation for creating the Node Simulator was to create a program that could send and receive
telemetry with the Flight Computer, while mimicing specific control behaviour for that node. 


### main.py
The entry point for the program. Here we 
- Create a behaviour
- Create a node with the behaviour
- Create a connection 
- Create our Message Handler
- Create our logger

 
### HardwareStateSimulation
This holds all of our node profiles. The base class enforces certain class variables a node must have.
Nodes only vary in their "dummy sensor values". 

### Behaviour
When a node is created, it is given a behaviour class. This dictates how the node will transition between
its states. A behaviour object contains a dict of transitions, where the key is the current state and the value
is the transition object. As we want the simulated node to be 100% deterministic, each state only has 1 transition.

### Transitions
There are various types of transitions to simulate node behaviour. Refrence the constructor of each transition
to understand how they work. 

##### Examples
In this transition creation, when the brake node is in STANDBY, it will transition to ARMING when the neighbor state 
is the FlightComputer state ARMING. Thus the transition is conditional on the neighbor state.
``` 
self.transitions[BrakeNodeStates.STANDBY] = ConditionalTransition(FlightComputerStates.ARMING, BrakeNodeStates.ARMING)
```

When the brake node is in ARMING, if the flight computer state stays ARMING for 4 seconds, the brake node state transitions
to ARMED.
```
self.transitions[BrakeNodeStates.ARMING] = TimedTransition(FlightComputerStates.ARMING, BrakeNodeStates.ARMED, 4)
```

### Message Handler
The message handler handel's converting serialized message data to the state variable that message represents and
vice versa. 

### States
Simple states enum objects. 

### Logger
Simple logginc class so that only changes in states is logged, simplifying output.

### Networking
Handels all UDP sending and receiving, 