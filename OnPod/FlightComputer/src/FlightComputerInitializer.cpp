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

void FlightComputerInitializer::updatePodNetworkValues(PodNetwork& podNetworkValues, flightConfig& config)
{
    std::string cNodeIpAddrs[] = {"192.168.0.50"};
    podNetworkValues.cNodeIpAddrs.assign(begin(cNodeIpAddrs), end(cNodeIpAddrs)); // Node IPs

    podNetworkValues.iBrakeNodePort = NetworkConstants::iBRAKE_NODE_PORT; // Port # that Nodes are listening on
    podNetworkValues.iNodeTimeoutMili = NetworkConstants::iNODE_TIMEOUT_MILI;
    podNetworkValues.iBrakeNodeServerPortNumber = NetworkConstants::iBRAKE_NODE_SERVER_PORT_NUMBER; // Port # to receive UDP from Nodes

    podNetworkValues.iCommaderTimeoutMili = config.heartbeattimeout(); // Timeout for heartbeat to Control Interface
    podNetworkValues.iCommanderPortNumber = config.commandport(); //Port # for TCP Commander

    podNetworkValues.iPdsTelemeteryPort = config.pdstelemetryport(); // Port # to send telemetry
    podNetworkValues.strPdsIpAddr = config.controllaptopipaddr(); // Ip Addr of PDS.

    podNetworkValues.iActiveNodes[0] = 1; // Set brake node active
}
