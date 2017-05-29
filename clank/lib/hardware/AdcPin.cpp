#include "AdcPin.hpp"

using namespace std;
using namespace Clank::Hardware;
using namespace Clank::Configuration;

AdcPin::AdcPin(string name, Configuration::Configuration *config)
  : AdcPin(SamPeripheralExtentions::StringToPeripheralName(name), config) {}

AdcPin::AdcPin(SamPeripheralName name, Clank::Configuration::Configuration* config)
  : SamPeripheral(name)
{
  string startingDelta;
  if (!config->GetConfigurationData("Threshold", startingDelta))
  {
    string message = "ADC Error: Invalid configuration for ADC ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += "\nConfiguration: <Threshold>";
    throw runtime_error(message);
  }

  /*Parse starting delta and set*/
  this->DeltaThresholdSetPoint = stod(startingDelta, NULL);
  if (this->DeltaThresholdSetPoint <= 0 || this->DeltaThresholdSetPoint >= 100)
  {
    string message = "ADC Error: Invalid configuration for ADC ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += "\nStartingDelta must be between 0 and 100\n";
    message += "(Read ";
    message += startingDelta;
    message += ")\n";
  }

  /*Set to a value that doesn't make sense so we can check for consistancy*/
  this->LastReportedDeltaThreshold = 0.0;
  this->SetState();
}

AdcPin::AdcPin(SamPeripheralName name, double threshold)
  : SamPeripheral(name)
{
  if (threshold <= 0.0 || threshold > 100.0)
  {
    string message = "ADC Error: Invalid configuration for ADC ";
    message += SamPeripheralExtentions::PeripheralNameToString(name);
    message += "\nStartingDelta must be between 0 and 100\n";
    message += "(Attempted set point: ";
    message += threshold;
    message += ")\n";
  }

  this->LastReportedDeltaThreshold = 0.0;
  this->SetState();
}

AdcPin::AdcPin(string name, double threshold)
  : AdcPin(SamPeripheralExtentions::StringToPeripheralName(name), threshold)
{}

AdcPin::~AdcPin()
{

}

void AdcPin::SetDeltaThreshold(double threshold)
{
  if (threshold <= 0.0 || threshold >= 100.0)
  {
    string message = "ADC Error: Attempting to set threshold for ADC ";
    message += SamPeripheralExtentions::PeripheralNameToString(this->GetName());
    message += " to a value outside the range (0.0, 100.0)";
    throw runtime_error(message);
  }
  this->DeltaThresholdSetPoint = threshold;
  this->SetState();
}

double AdcPin::GetDeltaThreshold()
{
  return this->LastReportedDeltaThreshold;
}

double AdcPin::GetValue()
{
  return this->LastReportedValue;
}

bool AdcPin::IsConsistant()
{
  return (this->DeltaThresholdSetPoint == this->LastReportedDeltaThreshold);
}

string AdcPin::StateToMessage()
{
  string message = SamPeripheralExtentions::PeripheralNameToString(this->GetName());
  message += ":thresh:";
  message += to_string(this->DeltaThresholdSetPoint);
  message += ":\n";
  return message;
}

void AdcPin::ParseMessage(string message)
{
  istringstream messageStream(message);
  string token;

  std::getline(messageStream, token, ':'); //get past name

  while(std::getline(messageStream, token, ':'))
  {
    if (!token.compare("thresh"))
    {
      getline(messageStream, token, ':');
      double receivedDelta = stod(token, NULL);
      if (receivedDelta <= 0.0 || receivedDelta >= 100.0)
      {
        string errorMessage = "ADC Error: Received invalid threshold delta from hardware.";
        errorMessage += "\nExpected value on range (0.0, 100.0), received ";
        errorMessage += token;
        throw runtime_error(errorMessage);
      }
      else
      {
        this->LastReportedDeltaThreshold = receivedDelta;
      }
    }

    else if (!token.compare("val"))
    {
      getline(messageStream, token, ':');
      double receivedVal = stod(token, NULL);
      if (receivedVal < 0.0 || receivedVal > 100.0)
      {
        string errorMessage = "ADC Error: Received invalid ADC value form hardware.";
        errorMessage += "\nExpected value on range (0.0, 100.0), received ";
        errorMessage += token;
        throw runtime_error(errorMessage);
      }
      this->LastReportedValue = receivedVal;
    }

    else if (!token.compare(SamPeripheralExtentions::PeripheralNameToString(this->GetName()))
              ||
              token.find_first_not_of(" :\t\n\r") == string::npos)
    {
      /*Do nothing*/
    }

    else
    {
      string message = "ADC Error: Invalid ADC token from SAM ";
      message += SamPeripheralExtentions::PeripheralNameToString(this->GetName());
      message += ": ";
      message += token;
      throw runtime_error(message);
    }
  }
}
