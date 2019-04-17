# Paradigm Data Shuttle

This program will connect to the Pod, receive telemetry packet, log them in InfluxDB, as well as generated and send
packets to SpaceX servers during the run.


## Stuff to look out for 
> The total length of the payload of the packet is **34bytes**. Frames with the wrong payload length will be ignored by 
SpaceX software. The Pod status field communicates the state of the Pod to SpaceX and the public and contains the 
following states:
> - 0: Fault – When the Pod is in any fault state.
>- 1: Safe to Approach –Any state where the Pod is on, not moving and safe for team members and SpaceX volunteers to 
approach. The Pod should go back to transmitting this state at the end of the run.
>- 2: Ready to Launch – Any state where the Pod is ready to launch but not accelerating.
>- 3: Launching – Any state where the Pod is commanding propulsion with the goal of achieving the highest velocity for the 
run.
>- 4: Coasting – Any state where the Pod is moving but not accelerating or decelerating.
>- 5: Braking – Any state where the Pod is decelerating using its braking mechanism, a Pod can stay in this state after 
having come to a full stop but should eventually go to Safe to approach
>- 6: Crawling – Any state where the Pod is commanding propulsion with the goal of moving the Pod while not achieving 
the highest velocity for the run.