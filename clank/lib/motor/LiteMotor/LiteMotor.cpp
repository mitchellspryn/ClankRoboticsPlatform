#include "LiteMotor.hpp"

using namespace std;
using namespace Clank::Hardware;
using namespace Clank::Configuration;
using namespace Clank::Motor;

LiteMotor::LiteMotor(Clank::Configuration::Configuration *configuration)
{
  string configVelocity, configRamp, dirPin, pwmPin;

  if (!configuration->GetConfigurationData("Velocity", configVelocity)
      ||
      !configuration->GetConfigurationData("RampSpeed", configRamp)
      ||
      !configuration->GetConfigurationData("DirPin", dirPin)
      ||
      !configuration->GetConfigurationData("PwmPin", pwmPin)
    )
  {
    string errorMessage = "LiteMotor error: invalid configuration\n";
    errorMessage += "Configuration: <Velocity> <RampSpeed> <DirPin> <PwmPin>\n";
    throw runtime_error(errorMessage);
  }

  /*Check input params*/
  int startingRampSpeed;
  double startingVelocity;
  try
  {
    startingVelocity = stod(configVelocity);
    if (startingVelocity < -100.0 || startingVelocity > 100.0)
    {
      throw runtime_error("");
    }
  }
  catch (...)
  {
    string errorMessage = "LiteMotor error: invalid configuration.\n";
    errorMessage += "Attempted to set starting velocity to an invalid value.\n";
    errorMessage += "Expected float on the range [-100.0, 100.0], got ";
    errorMessage += configVelocity;
    throw runtime_error(errorMessage);
  }
  try
  {
    startingRampSpeed = stoi(configRamp);
    if (startingRampSpeed == 0)
      throw runtime_error("");
  }
  catch (...)
  {
    string errorMessage = "LiteMotor error: invalid configuration.\n";
    errorMessage += "Attempted to set starting ramp speed to an invalid value.\n";
    errorMessage += "Expected non-zero integer, got ";
    errorMessage += startingRampSpeed;
    throw runtime_error(errorMessage);
  }

  /*Initialize lower level hardware members with default values - motor stopped*/
  this->DirPin = new GpioPin(dirPin, Clank::Hardware::GpioDirection::Output, Clank::Hardware::GpioValue::Low);
  this->PulsePin = new PwmPin(pwmPin, -1.0, 0.0);

  /*Set parameters*/
  this->SetRampSpeed(startingRampSpeed);
  this->SetVelocity(startingVelocity);
}

LiteMotor::LiteMotor(double startingRampSpeed, double startingVelocity, string dirControlName, string pwmPinName)
{
  /*Check parameters*/
  if (startingVelocity < -100.0 || startingVelocity > 100.0)
  {
    string errorMessage = "LiteMotor error: invalid constructor argument.\n";
    errorMessage += "Attempted to set starting velocity to an invalid value.\n";
    errorMessage += "Expected float on the range [-100.0, 100.0], got ";
    errorMessage += to_string(startingVelocity);
    throw runtime_error(errorMessage);
  }

  if (startingRampSpeed == 0)
  {
    string errorMessage = "LiteMotor error: invalid constructor argument.\n";
    errorMessage += "Attempted to set starting ramp speed to an invalid value.\n";
    errorMessage += "Expected non-zero integer, got ";
    errorMessage += startingRampSpeed;
    throw runtime_error(errorMessage);
  }

  /*Initialize lower level hardware members with default values - motor stopped*/
  this->DirPin = new GpioPin(dirControlName, Clank::Hardware::GpioDirection::Output, Clank::Hardware::GpioValue::Low);
  this->PulsePin = new PwmPin(pwmPinName, -1.0, 0.0);

  /*Set parameters*/
  this->SetRampSpeed(startingRampSpeed);
  this->SetVelocity(startingVelocity);
}

LiteMotor::~LiteMotor()
{
  delete this->DirPin;
  delete this->PulsePin;
}

double LiteMotor::GetVelocity()
{
  double velocityMag = this->PulsePin->GetValue();

  if (this->DirPin->GetValue() == Clank::Hardware::GpioValue::High)
  {
    return velocityMag;
  }

  return -1*velocityMag;
}

void LiteMotor::SetVelocity(double velocity)
{
  if (velocity < -100.0 || velocity > 100.0)
  {
    string errorMessage = "LiteMotor error: invalid velocity.\n";
    errorMessage += "Attempted to set velocity to an invalid value.\n";
    errorMessage += "Expected float on the range [-100.0, 100.0], got ";
    errorMessage += to_string(velocity);
    throw runtime_error(errorMessage);
  }

  if (velocity < 0)
  {
    this->DirPin->SetValue(Clank::Hardware::GpioValue::Low);
    velocity *= -1;
  }
  else if (velocity > 0)
  {
    this->DirPin->SetValue(Clank::Hardware::GpioValue::High);
  }

  this->PulsePin->SetValue(velocity);
  return;
}

double LiteMotor::GetRampSpeed()
{
  return this->PulsePin->GetRampSpeed();
}

void LiteMotor::SetRampSpeed(double speed)
{
  if (speed == 0)
  {
    string errorMessage = "LiteMotor error: invalid ramp speed.\n";
    errorMessage += "Attempted to set ramp speed to an invalid value.\n";
    errorMessage += "Expected non-zero integer, got ";
    errorMessage += to_string(speed);
    throw runtime_error(errorMessage);
  }

  this->PulsePin->SetRampSpeed(speed);

}


