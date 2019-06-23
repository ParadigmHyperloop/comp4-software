#include "FlightComputerInitializer.h"
#include "FileHelper.h"
#include "Constants/Constants.h"
#include "Common.h"


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
    std::ifstream infile("/home/debian/logging.conf");
    if (infile.good())
    {
        return;
        el::Configurations conf("/home/debian/logging.conf");
        el::Loggers::reconfigureAllLoggers(conf);
    } else
    {
        std::string loggerLibraryImportPath = FileHelper::GetCurrentDirectory() +"/../../libs/EasyLogger/logging.conf";
        el::Configurations conf(loggerLibraryImportPath);
        el::Loggers::reconfigureAllLoggers(conf);
    }
}

void FlightComputerInitializer::updatePodNetworkValues(PodNetwork& podNetworkValues, FlightConfig& config)
{
    std::string cNodeIpAddrs[] = {"192.168.1.20"};
    podNetworkValues.cNodeIpAddrs.assign(begin(cNodeIpAddrs), end(cNodeIpAddrs)); // Node IPs

    podNetworkValues.iBrakeNodePort = config.brakenodeport(); // Port # that Nodes are listening on
    podNetworkValues.iNodeTimeoutMili = config.brakenodetimeout();
    podNetworkValues.iBrakeNodeServerPortNumber = config.brakenodeserverport(); // Port # to receive UDP from Nodes

    podNetworkValues.iCommaderTimeoutMili = config.heartbeattimeout(); // Timeout for heartbeat to Control Interface
    podNetworkValues.iCommanderPortNumber = config.commandport(); //Port # for TCP Commander

    podNetworkValues.iPdsTelemeteryPort = config.pdstelemetryport(); // Port # to send telemetry
    podNetworkValues.strPdsIpAddr = config.controllaptopipaddr(); // Ip Addr of PDS.

    podNetworkValues.iActiveNodes[0] = 1; // Set brake node active
}
