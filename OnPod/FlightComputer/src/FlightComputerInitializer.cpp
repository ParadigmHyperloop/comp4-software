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
    std::string cNodeIpAddrs[] = {"192.168.0.30","192.168.0.31","192.168.0.32"}; // Brake , LVDC, Enclosure
    podNetworkValues.cNodeIpAddrs.assign(begin(cNodeIpAddrs), end(cNodeIpAddrs)); // Node IPs
    podNetworkValues.strPdsIpAddr = config.controllaptopipaddr(); // Ip Addr of PDS.

    podNetworkValues.nodePort = config.nodeport(); // Port # that Nodes are listening on
    podNetworkValues.nodeTimeoutMili = config.nodetimeout();
    podNetworkValues.brakeNodeServerPortNumber = config.brakenodeserverport(); // Port # to receive UDP from Nodes
    podNetworkValues.enclosureNodeServerNumber = config.enclosurenodeservernumber();
    podNetworkValues.lvdcNodeServerNumber = config.lvdcnodeservernumber();

    podNetworkValues.commaderTimeoutMili = config.heartbeattimeout(); // Timeout for heartbeat to Control Interface
    podNetworkValues.commanderPortNumber = config.commandport(); //Port # for TCP Commander

    podNetworkValues.brakeNodeUpdateFreq = config.brakenodeupdatefreq();
    podNetworkValues.pdsUpdateFreq = config.pdsupdatefreq();
    podNetworkValues.pdsTelemeteryPort = config.pdstelemetryport(); // Port # to send telemetry
}

SevenSegStateDisplay* FlightComputerInitializer::GetSevenSegStateDisplay(TelemetryManager* manger)
{
  el::Helpers::setThreadName("main");
  std::ifstream infile("/home/debian/logging.conf");
  bool onBeagle = false;
  if (infile.good())
  {
    onBeagle = true;
  }

  return new SevenSegStateDisplay(manger, onBeagle);
}

