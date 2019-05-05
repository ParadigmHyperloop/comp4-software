#ifndef FLIGHTCOMPUTER_FLIGHTCOMPUTERINITIALIZER_H
#define FLIGHTCOMPUTER_FLIGHTCOMPUTERINITIALIZER_H



// TODO: Make thread safe
class FlightComputerInitializer {

private:
    static FlightComputerInitializer *_flightComputerInitializer;
    FlightComputerInitializer operator=(const FlightComputerInitializer&);
public:
    FlightComputerInitializer();


public:
    static FlightComputerInitializer* GetInstance();
    void importLoggerLibrary();



};


#endif
