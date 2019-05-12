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

void FlightComputerInitializer::updatePodNetworkValues(PodNetwork& podNetworkValues, flightConfig& config, char* controlLaptopAddr)
{
    std::string cNodeIpAddrs[] = {"127.0.0.1"};
    podNetworkValues.cNodeIpAddrs.assign(begin(cNodeIpAddrs), end(cNodeIpAddrs)); // Node IPs

    podNetworkValues.iBrakeNodePort = NetworkConstants::iBRAKE_NODE_PORT; // Port # that Nodes are listening on
    podNetworkValues.iNodeTimeoutMili = NetworkConstants::iNODE_TIMEOUT_MILI;
    podNetworkValues.iBrakeNodeServerPortNumber = NetworkConstants::iBRAKE_NODE_SERVER_PORT_NUMBER; // Port # to receive UDP from Nodes

    podNetworkValues.iCommaderTimeoutMili = config.heartbeat_timeout(); // Timeout for heartbeat to Control Interface
    podNetworkValues.iCommanderPortNumber = config.command_port(); //Port # for TCP Commander

    podNetworkValues.iPdsTelemeteryPort = config.telemetry_port(); // Port # to send telemetry
    podNetworkValues.strPdsIpAddr = std::string(controlLaptopAddr); // Ip Addr of PDS.

    podNetworkValues.iActiveNodes[0] = 1; // Set brake node active
}
