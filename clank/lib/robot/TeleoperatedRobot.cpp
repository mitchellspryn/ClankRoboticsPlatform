#include "TeleoperatedRobot.hpp"

#include <iostream>

using namespace std;
using namespace Clank::Configuration;
using namespace Clank::Communication;
using namespace Clank::Timers;
using namespace Clank::Module;
using namespace Clank::Robot;

TeleoperatedRobot::TeleoperatedRobot()
{

}

TeleoperatedRobot::~TeleoperatedRobot()
{
  this->VideoStreamer->StopStream();
  this->TcpServer->KillServer();
  delete this->VideoStreamer;
  delete this->Watchdog;
  delete this->DifferentialDriveModule;
  delete this->TcpServer;
}

bool TeleoperatedRobot::ConstructFromFile(std::string configurationFilePath)
{
  /*read in the configuration from file*/
  IndentedFileConfigurationReader reader;
  cout << "Reading config..." << endl;
  Clank::Configuration::Configuration* config = reader.ReadFromFile(configurationFilePath);
  cout << "Config read." << endl;

  /*Find the configuration corresponding to the watchdog and pass it to the watchdog*/
  Clank::Configuration::Configuration currentConfig;
  cout << "Starting timer init.." << endl;
  if (!config->FindConfigurationByTag("Watchdog", currentConfig))
  {
    string errorMessage = "TeleoperatedRobot error: missing <Watchdog> attribute from configuration.";
    cout << errorMessage << endl;
    throw runtime_error(errorMessage);
  }

  this->Watchdog = new Clank::Timers::Watchdog(&currentConfig);
  function<void(void)> watchdogCallback = bind(&TeleoperatedRobot::OnWatchdogTimeout, this);
  this->Watchdog->AddTimeoutCallback(watchdogCallback);

  cout << "Timer init done. Starting DDM init..." << endl;

  /*Find the configuration corresponding to the drive motor controller and pass it to the controller*/
  if (!config->FindConfigurationByTag("DifferentialDriveModule", currentConfig))
  {
    string errorMessage = "TeleoperatedRobot error: missing <DifferentialDriveModule> attribute from configuration.";
    cout << errorMessage << endl;
    throw runtime_error(errorMessage);
  }

  this->DifferentialDriveModule = new Clank::Module::DifferentialDriveModule(&currentConfig);

  cout << "DDM Init done. Starting TCP server init..." << endl;
  /*Find the configuration corresponding to the Tcp server*/
  if (!config->FindConfigurationByTag("TcpServer", currentConfig))
  {
    string errorMessage = "TeleoperatedRobot error: missing <TcpServer> attribute from configuration.";
    throw runtime_error(errorMessage);
  }

  this->TcpServer = new Clank::Communication::TcpServer(&currentConfig);

  cout << "Starting TCP Server..." << endl;

  function<const char*(Clank::Communication::Command*)> serverCallback = bind(&TeleoperatedRobot::OnCommandReceived, this, std::placeholders::_1);
  this->TcpServer->AddListener(serverCallback);
  this->TcpServer->Start();

  cout << "TcpServer init done. Starting VideoStreamer..." << endl;

  if (!config->FindConfigurationByTag("VideoStreamer", currentConfig))
  {
    string errorMessage = "TeleoperatedRobot error: missing <VideoStreamer> attribute from configuration.";
    throw runtime_error(errorMessage);
  }

  this->VideoStreamer = new Clank::Module::VideoStreamer(&currentConfig);


  cout << "Starting Video stream..." << endl;

  this->VideoStreamer->Initialize();
  this->VideoStreamer->StartStream();

  cout << "Starting watchdog..." << endl;
  this->Watchdog->Start();

  cout << "Initialized!" << endl;
  return true;
}

void TeleoperatedRobot::OnWatchdogTimeout()
{
  if (!this->TcpServer->isConnected() || this->TcpServer->isAlive())
  {
    cout << "Robot watchdog timeout." << endl;
    this->DifferentialDriveModule->Stop();
  }
  else
  {
    throw runtime_error("TeleoperatedRobot Error: Server died, detected by Watchdog Timeout.");
  }
}

/*TODO: Refactor to match style of the rest of the code base*/
const char* TeleoperatedRobot::OnCommandReceived(Command* command)
{
  /*Kick the watchdog*/
  this->Watchdog->Kick();

  /*Attempt to parse*/
  if (!strcmp(command->getModule(), "Base")
      &&
      !strcmp(command->getComponent(), "base")
      &&
      !strcmp(command->getProperty(), "drive")
    )
  {
    string value = string(command->getVal());
    int splitPos = value.find_first_of("|");

    try
    {
      double r = stod(value.substr(0, splitPos-1));
      double theta = stod(value.substr(splitPos+1, string::npos));
      this->DifferentialDriveModule->Drive(r, theta);
      return "OK"; 
    }
    catch (...)
    {
      return "ERROR";
    }
  }

  return "";

}
