#include "FlightComputerInitializer.h"
#include "EasyLogger/easylogging++.h"
#include "Helpers/FileHelper.h"



FlightComputerInitializer* FlightComputerInitializer::_flightComputerInitializer = NULL;

FlightComputerInitializer::FlightComputerInitializer() = default;

FlightComputerInitializer* FlightComputerInitializer::GetInstance()
{
    if (_flightComputerInitializer == NULL)
        _flightComputerInitializer = new FlightComputerInitializer();
    return _flightComputerInitializer;
}


void FlightComputerInitializer::importLoggerLibrary()
{
    el::Helpers::setThreadName("main");
    std::ifstream infile("home/debian/logging.conf");

    if (infile.good())
    {
        el::Configurations conf("home/debian/logging.conf");
        el::Loggers::reconfigureAllLoggers(conf);
    } else
    {
        std::string loggerLibraryImportPath = FileHelper::GetCurrentDirectory() +"/../../libs/EasyLogger/logging.conf";
        el::Configurations conf(loggerLibraryImportPath);
        el::Loggers::reconfigureAllLoggers(conf);
    }
}
