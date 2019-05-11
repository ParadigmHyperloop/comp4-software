#include "FlightComputerInitializer.h"
#include "EasyLogger/easylogging++.h"
#include "Helpers/FileHelper.h"
#include "Constants/Constants.h"


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

void FlightComputerInitializer::updatePodNetworkValues(PodNetwork& s, flightConfig& config, char* controlLaptopAddr)
{
    std::string cNodeIpAddrs[] = {"127.0.0.1"};
    s.cNodeIpAddrs.assign(begin(cNodeIpAddrs), end(cNodeIpAddrs)); // Node IPs

    s.iBrakeNodePort = NetworkConstants::iBRAKE_NODE_PORT; // Port # that Nodes are listening on
    s.iNodeTimeoutMili = NetworkConstants::iNODE_TIMEOUT_MILI;
    s.iBrakeNodeServerPortNumber = NetworkConstants::iBRAKE_NODE_SERVER_PORT_NUMBER; // Port # to receive UDP from Nodes

    s.iCommaderTimeoutMili = config.heartbeat_timeout(); // Timeout for heartbeat to Control Interface
    s.iCommanderPortNumber = config.command_port(); //Port # for TCP Commander

    s.iPdsTelemeteryPort = config.telemetry_port(); // Port # to send telemetry
    s.strPdsIpAddr = std::string(controlLaptopAddr); // Ip Addr of PDS.

    s.iActiveNodes[0] = 1; // Set brake node active

}
