#ifndef FLIGHTCOMPUTER_FLIGHTCOMPUTERINITIALIZER_H
#define FLIGHTCOMPUTER_FLIGHTCOMPUTERINITIALIZER_H

#include "Structs.h"
class FlightComputerInitializer {

private:
    static FlightComputerInitializer *_flightComputerInitializer;
    FlightComputerInitializer operator=(const FlightComputerInitializer&);
    FlightComputerInitializer();

public:
    static FlightComputerInitializer* GetInstance();
    void importLoggerLibrary();
    void updatePodNetworkValues(PodNetwork&, flightConfig& config);



};


#endif
