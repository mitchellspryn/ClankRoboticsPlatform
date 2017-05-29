#include "GpioPin.hpp"

using namespace std;
using namespace Clank::Hardware;
using namespace Clank::Configuration;

GpioPin::GpioPin(string name, Configuration::Configuration *config)
 : GpioPin(SamPeripheralExtentions::StringToPeripheralName(name), config) {}

GpioPin::GpioPin(SamPeripheralName name, Configuration::Configuration *config)
  : SamPeripheral(name)
{
  string startingDirection, startingValue;
  if  ((!config->GetConfigurationData("Direction", startingDirection)) ||
       (!config->GetConfigurationData("Value", startingValue)))
  {
    string message = "GPIO Error: Invalid configuration for GPIO ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += "\nConfiguration: <Direction> <Value>";
    throw runtime_error(message);
  } 

  /*Parse direction*/
  if (!startingDirection.compare("In"))
  {
    this->StagingDirection = Input;
  }
  else if (!startingDirection.compare("Out"))
  {
    this->StagingDirection = Output;
  }
  else
  {
    string message = "GPIO Error: Invalid direction for GPIO ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += ": ";
    message += startingDirection;
    message += "\nValid options: <In> <Out>";
    throw runtime_error(message);
  }

  /*Parse value*/
  if (!startingValue.compare("High"))
  {
    this->StagingValue = High;
  }
  else if (!startingValue.compare("Low"))
  {
    this->StagingValue = Low;
  }
  else
  {
    string message = "GPIO Error: Invalid value for GPIO ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += ": ";
    message += startingValue;
    message += "\nValid options: <High> <Low>";
    throw runtime_error(message);
  }

  /*Set the current state to something that doesn't match so we can check consistancy*/
  (this->StagingDirection == Input? this->Direction = Output : this->Direction = Input);
  (this->StagingValue == High ? this->Value = Low : this->Value = High);

  this->SetState();
}

GpioPin::GpioPin(SamPeripheralName name, GpioDirection startingDirection, GpioValue startingValue)
  : SamPeripheral(name)
{
  this->StagingDirection = startingDirection;
  this->StagingValue = startingValue;

  (this->StagingDirection == Input? this->Direction = Output : this->Direction = Input);
  (this->StagingValue == High ? this->Value = Low : this->Value = High);

  this->SetState();
}

GpioPin::GpioPin(string name, GpioDirection startingDirection, GpioValue startingValue)
  : GpioPin(SamPeripheralExtentions::StringToPeripheralName(name), startingDirection, startingValue)
{}

GpioPin::~GpioPin()
{}

void GpioPin::SetDirection(GpioDirection direction)
{
  this->StagingDirection = direction;
  this->SetState();
}

GpioDirection GpioPin::GetDirection()
{
  return this->Direction;
}

void GpioPin::SetValue(Clank::Hardware::GpioValue value)
{
  this->StagingValue = value;
  this->SetState();
}

GpioValue GpioPin::GetValue()
{
  return this->Value;
}

string GpioPin::StateToMessage()
{
  string message = SamPeripheralExtentions::PeripheralNameToString(this->GetName());
  message += ":dir:";

  if (this->StagingDirection == Input)
    message += "In";
  else if (this->StagingDirection == Output)
    message += "Out";
  else
    throw runtime_error("Unrecognized state for GPIO direction");

  message += ":val:";
  if (this->StagingValue == High)
    message += "Hi:\n";
  else if (this->StagingValue == Low)
    message += "Low:\n";
  else
    throw runtime_error("Unrecognized state for GPIO Value");
  
  return message;
}

void GpioPin::ParseMessage(std::string message)
{
  istringstream messageStream(message);
  string token;

  std::getline(messageStream, token, ':'); //get past name

  while(std::getline(messageStream, token, ':'))
  {
    if (!token.compare("dir"))
    {
      std::getline(messageStream, token, ':');

      if (!token.compare("In"))
      {
        this->Direction = Input;
      }
      else if (!token.compare("Out"))
      {
        this->Direction = Output;
      }
      else
      {
        string message = "GPIO Error: Received invalid GPIO token from SAM ";
        message += SamPeripheralExtentions::PeripheralNameToString(this->GetName());
        message += ": ";
        message += token;
        message += "\nValid options: <In> <Out>";
        throw runtime_error(message);
      }
    }

    else if (!token.compare("val")) //TODO: else if?
    {
      getline(messageStream, token, ':');

      if (!token.compare("Hi"))
      {
        this->Value = High;
      }
      else if (!token.compare("Low"))
      {
        this->Value = Low;
      }
      else
      {
        string message = "GPIO Error: Invalid GPIO token from SAM ";
        message += SamPeripheralExtentions::PeripheralNameToString(this->GetName());
        message += ": ";
        message += token;
        message += "\nValid options: <High> <Low>";
        throw runtime_error(message);
      }
    }

    else if (!token.compare(SamPeripheralExtentions::PeripheralNameToString(this->GetName()))
              ||
              token.find_first_not_of(" :\t\n\r") == string::npos)
    {
      /*Do nothing*/
    }

    else
    {
      string message = "GPIO Error: Invalid GPIO token from SAM ";
      message += SamPeripheralExtentions::PeripheralNameToString(this->GetName());
      message += ": ";
      message += token;
      throw runtime_error(message);
    }

  }
}

bool GpioPin::IsConsistant()
{
  return (
          (this->StagingDirection == this->Direction)
          &&
          (
            (this->StagingValue == this->Value)
            ||
            (this->Direction == Input)
          )
        );
}
