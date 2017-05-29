#include "PwmPin.hpp"

using namespace std;
using namespace Clank::Hardware;
using namespace Clank::Configuration;

PwmPin::PwmPin(string name, Clank::Configuration::Configuration *config)
  : PwmPin(SamPeripheralExtentions::StringToPeripheralName(name), config) {};

PwmPin::PwmPin(Clank::Hardware::SamPeripheralName name, Clank::Configuration::Configuration* config)
  : SamPeripheral(name)
{
  string startingRampSpeed, startingValue;
  if ((!config->GetConfigurationData("RampSpeed", startingRampSpeed)) ||
      (!config->GetConfigurationData("Value", startingValue)))
  {
    string message = "PwmPin Error: Invalid configuration for PwmPin ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += "\nConfiguration: <RampSpeed> <Value>";
    throw runtime_error(message);
  }

  /*Parse RampSpeed*/
  this->StagingRampSpeed = stod(startingRampSpeed, NULL);
  if (!this->StagingRampSpeed)
  {
    string message = "PwmPin error: Invalid Starting Ramp Speed for PwmPin ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += "\nExpected nonzero float, received ";
    message += startingRampSpeed;
    throw runtime_error(message);
  }

  /*Parse initial value*/
  this->StagingValue = stod(startingValue);
  if ( (!this->StagingValue && startingValue.find_first_not_of("0.\n") != string::npos) || this->StagingValue < 0.0 || this->StagingValue > 100.0) //TODO: better error checking?
  {
    string message = "PwmPin error: Invalid Starting Value for PwmPin ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += "\nExpected float on range of (0.0, 100.0), received ";
    message += startingValue;
    throw runtime_error(message);
  }

  /*Set to values that don't make sense to check consistancy*/
  this->LastReportedRampSpeed = -1.0;
  this->LastReportedValue = -1.0;

  this->SetState();
}

PwmPin::PwmPin(Clank::Hardware::SamPeripheralName name, double startingRampSpeed, double startingValue)
  : SamPeripheral(name)
{
  this->StagingRampSpeed = startingRampSpeed;

  if (startingValue < 0.0 || startingValue > 100.0)
  {
    string message = "PwmPin error: Invalid Starting Value for PwmPin ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += "\nExpected float on range of (0.0, 100.0), received ";
    message += startingValue;
    throw runtime_error(message);
  }

  this->StagingValue = startingValue;

  this->LastReportedRampSpeed = -1.0;
  this->LastReportedValue = -1.0;

  this->SetState();
}

PwmPin::PwmPin(string name, double startingRampSpeed, double startingValue)
 : PwmPin(SamPeripheralExtentions::StringToPeripheralName(name), startingRampSpeed, startingValue)
{}

PwmPin::~PwmPin()
{
}

void PwmPin::SetRampSpeed(double speed)
{
  if (speed == 0)
  {
    string errorMessage = "PwmPin error: Attempting to set ramp speed for PWM Pin ";
    errorMessage += SamPeripheralExtentions::PeripheralNameToString(this->GetName());
    errorMessage += " to 0.";
    throw runtime_error(errorMessage);
  }

  this->StagingRampSpeed = speed;
  this->SetState();
}

double PwmPin::GetRampSpeed()
{
  return this->LastReportedRampSpeed;
}

void PwmPin::SetValue(double value)
{
  if (value < 0.0 || value > 100.0)
  {
    string errorMessage = "PwmPin error: Attempting to set PWM value for pin ";
    errorMessage += SamPeripheralExtentions::PeripheralNameToString(this->GetName());
    errorMessage += " to a value outside the range of (0.0, 100.0)";
    errorMessage += "\nValue requested: ";
    errorMessage += to_string(value);
    throw runtime_error(errorMessage);
  }

  this->StagingValue = value;
  this->SetState();
}

double PwmPin::GetValue()
{
  return this->LastReportedValue; 
}

bool PwmPin::IsConsistant()
{
  return (
          (this->LastReportedRampSpeed == this->StagingRampSpeed)
          &&
          (this->LastReportedValue == this->StagingValue)
         );
}

string PwmPin::StateToMessage()
{
  string message = SamPeripheralExtentions::PeripheralNameToString(this->GetName());
  message += ":val:";
  message += to_string(this->StagingValue);
  message += ":ramp:";
  message += to_string(this->StagingRampSpeed);
  message += ":\n";
  return message;
}

void PwmPin::ParseMessage(std::string message)
{
  istringstream messageStream(message);
  string token;

  std::getline(messageStream, token, ':');

  while(std::getline(messageStream, token, ':'))
  {
    if (!token.compare("val"))
    {
      getline(messageStream, token, ':');
      double receivedVal = stod(token, NULL);
      if (receivedVal < 0.0 || receivedVal > 100.0)
      {
        string errorMessage = "PWM Error: Received invalid PWM value from hardware.";
        errorMessage += "\nExpected value on range [0.0, 100.0], received ";
        errorMessage += token;
        throw runtime_error(message);
      }
      this->LastReportedValue = receivedVal;
    }

    else if (!token.compare("ramp"))
    {
      getline(messageStream, token, ':');
      double receivedVal = stod(token, NULL);
      if (!receivedVal)
      {
        string errorMessage = "Pwm Error: Received invalid PWM ramp speed from hardware.";
        errorMessage += "\nExpected nonzero value, got ";
        errorMessage += token;
        throw runtime_error(message);
      }
      this->LastReportedRampSpeed = receivedVal;
    }

    else if (!token.compare("set"))
    {
      getline(messageStream, token, ':');
      /*Nothing to do for this one*/
    }

    else if (!token.compare(SamPeripheralExtentions::PeripheralNameToString(this->GetName()))
              ||
              token.find_first_not_of(" :\t\n\r") == string::npos)
    {
      /*Do nothing*/
    }

    else
    {
      string message = "PWM Error: Invalid PWM token from SAM ";
      message += SamPeripheralExtentions::PeripheralNameToString(this->GetName());
      message += ": ";
      message += token;
      throw runtime_error(message);
    }
  }
}
