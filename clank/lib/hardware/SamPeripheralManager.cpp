#include "SamPeripheralManager.hpp"

using namespace std;
using namespace Clank::Hardware;

SamPeripheralManager* SamPeripheralManager::SamPeripheralManagerSingletonInstance = NULL;

SamPeripheralManager* SamPeripheralManager::Instance()
{
  if (SamPeripheralManagerSingletonInstance == NULL)
  {
    SamPeripheralManagerSingletonInstance = new SamPeripheralManager();
  }

  return SamPeripheralManagerSingletonInstance;
}

void SamPeripheralManager::RouteMessage(std::string message)
{
  int firstSeparatorIndex = message.find_first_of(':');
  SamPeripheralName targetPeripheral = SamPeripheralExtentions::StringToPeripheralName(message.substr(0, firstSeparatorIndex));
  
  if (this->EnabledStates.find(targetPeripheral) != this->EnabledStates.end())
  {
    if (message.find("Enabled") != string::npos)
    {
      this->EnabledStates[targetPeripheral].ActualState = true;
    }
    else if (message.find("Disabled") != string::npos)
    {
      this->EnabledStates[targetPeripheral].ActualState = false;
    }

    else
    {
      string errorMessage = "SamPeripheralManager Error: Attempting to parse update message ";
      errorMessage += message;
      errorMessage += " for peripheral ";
      errorMessage += SamPeripheralExtentions::PeripheralNameToString(targetPeripheral);
      errorMessage += " failed.";
      throw new runtime_error(errorMessage);
    }
    this->EnabledStates[targetPeripheral].UpdatedByHardware = true;
  }

  else if (this->PeripheralMap[targetPeripheral] == NULL)
  {
    string errorMessage = "SamPeripheralManager Error: Unable to parse message ";
    errorMessage += message;
    errorMessage += "\nDevice ";
    errorMessage += message.substr(0, firstSeparatorIndex-1);
    errorMessage += " not checked out.\n";
    throw runtime_error(errorMessage);
  }

  else
  {
    this->PeripheralMap[targetPeripheral]->ParseMessage(message);
  }
}

void SamPeripheralManager::Get(Clank::Hardware::SamPeripheralName name, Clank::Hardware::SamPeripheral *peripheralInstance)
{
  /*Check if already checked out*/
  if (this->PeripheralMap[name] != NULL)
  {
    string message = "SamPeripheralManager Error: Attempting to check out peripheral ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += " twice.";
    throw runtime_error(message);
  }

  string response;
  string message = SamPeripheralExtentions::PeripheralNameToString(name);
  message += ":enable:\n";
  this->SamSerialPort->SendMessage(message);

  RequestType r = {true, false, false};
  this->EnabledStates[name] = r;

  int waitCount = 0;
  while(this->EnabledStates[name].UpdatedByHardware == false)
  {
    usleep(1000);
    if (waitCount > 10)
    {
      string message = "SamPeripheralManager Error: Unable to allocate peripheral ";
      message += SamPeripheralExtentions::PeripheralNameToString(name);
      message += "\nSerial port timeout.";
      throw runtime_error(message);
    }
  }

  if (!(this->EnabledStates[name].ActualState))
  {
    string message = "SamPeripheralManager Error: Unable to allocate peripheral ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += "\nThe peripheral refused to be allocated.";
    throw runtime_error(message);
  }

  this->EnabledStates.erase(name);
  this->PeripheralMap[name] = peripheralInstance; 
}

void SamPeripheralManager::Return(Clank::Hardware::SamPeripheralName name)
{
  
  string response;
  string message = SamPeripheralExtentions::PeripheralNameToString(name);
  message += ":disable:\n";
  this->SamSerialPort->SendMessage(message);

  RequestType r = {false, false, false};
  this->EnabledStates[name] = r;

  int waitCount = 0;
  while(this->EnabledStates[name].UpdatedByHardware == false)
  {
    usleep(1000);
    if (waitCount > 10)
    {
      string message = "SamPeripheralManager Error: Unable to deallocate peripheral ";
      message += SamPeripheralExtentions::PeripheralNameToString(name);
      message += "\nSerial port timeout.";
      throw runtime_error(message);
    }
  }

  if (this->EnabledStates[name].ActualState)
  {
    string message = "SamPeripheralManager Error: Unable to deallocate peripheral ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += "\nThe peripheral refused to be deallocated.";
    throw runtime_error(message);
  }

  this->EnabledStates.erase(name);
  this->PeripheralMap[name] = NULL;
}

void SamPeripheralManager::InitializeSerialPort()
{
  this->SamSerialPort = SamSerialPort::Instance();
}

SamPeripheralManager::SamPeripheralManager()
{
}
