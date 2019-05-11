#ifndef FLIGHTCOMPUTER_FLIGHTCOMPUTERINITIALIZER_H
#define FLIGHTCOMPUTER_FLIGHTCOMPUTERINITIALIZER_H

class FlightComputerInitializer {

private:
    static FlightComputerInitializer *_flightComputerInitializer;
    FlightComputerInitializer operator=(const FlightComputerInitializer&);
    FlightComputerInitializer();

public:
    static FlightComputerInitializer* GetInstance();
    void importLoggerLibrary();



};


#endif
