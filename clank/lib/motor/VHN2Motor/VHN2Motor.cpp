#include "VHN2Motor.hpp"

using namespace std;
using namespace Clank::Configuration;
using namespace Clank::Hardware;
using namespace Clank::Motor;

VHN2Motor::VHN2Motor(Clank::Configuration::Configuration *configuration)
{
  string configVelocity, configRamp, gpioA, gpioB, pwmPin;
  
  if (!configuration->GetConfigurationData("Velocity", configVelocity)
      || 
      !configuration->GetConfigurationData("RampSpeed", configRamp)
      ||
      !configuration->GetConfigurationData("GPIOA", gpioA)
      ||
      !configuration->GetConfigurationData("GPIOB", gpioB)
      ||
      !configuration->GetConfigurationData("PWMPin", pwmPin)
    )
  {
    string errorMessage = "VHN2 motor error: invalid configuration.\n";
    errorMessage += "Configuration: <Velocity> <RampSpeed> <GPIOA> <GPIOB> <PWMPin>";
    throw runtime_error(errorMessage);
  }

  /*Check input parameters*/
  int startingRampSpeed;
  double startingVelocity;
  try
  {
    startingVelocity = stod(configVelocity); 
    if (startingVelocity < -100.0 || startingVelocity > 100.0)
      throw runtime_error(""); //caught below
  }
  catch (...)
  {
    string errorMessage = "VHN2 motor error: invalid configuration.\n";
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
    string errorMessage = "VHN2 motor error: invalid configuration.\n";
    errorMessage += "Attempted to set starting ramp speed to an invalid value.\n";
    errorMessage += "Expected non-zero integer, got ";
    errorMessage += startingRampSpeed;
    throw runtime_error(errorMessage);
  }

  /*Initialize lower level hardware members with default values - motor stopped*/
  this->PinA = new GpioPin(gpioA, Clank::Hardware::GpioDirection::Output, Clank::Hardware::GpioValue::Low);
  this->PinB = new GpioPin(gpioB, Clank::Hardware::GpioDirection::Input, Clank::Hardware::GpioValue::High);
  this->PulsePin = new PwmPin(pwmPin, -1.0, 0.0);

  /*Set parameters*/
  this->SetRampSpeed(startingRampSpeed);
  this->SetVelocity(startingVelocity);
}

VHN2Motor::VHN2Motor(double startingRampSpeed, double startingVelocity, string gpioAName, string gpioBName, string pwmPinName)
{
  /*Check parameters*/
  if (startingVelocity < -100.0 || startingVelocity > 100.0)
  {
    string errorMessage = "VHN2 motor error: invalid constructor argument.\n";
    errorMessage += "Attempted to set starting velocity to an invalid value.\n";
    errorMessage += "Expected float on the range [-100.0, 100.0], got ";
    errorMessage += to_string(startingVelocity);
    throw runtime_error(errorMessage);
  }

  if (startingRampSpeed == 0)
  {
    string errorMessage = "VHN2 motor error: invalid constructor argument.\n";
    errorMessage += "Attempted to set starting ramp speed to an invalid value.\n";
    errorMessage += "Expected non-zero integer, got ";
    errorMessage += startingRampSpeed;
    throw runtime_error(errorMessage);
  }

  /*Initialize lower level hardware members with default values - motor stopped*/
  this->PinA = new GpioPin(gpioAName, Clank::Hardware::GpioDirection::Output, Clank::Hardware::GpioValue::Low);
  this->PinB = new GpioPin(gpioBName, Clank::Hardware::GpioDirection::Input, Clank::Hardware::GpioValue::High);
  this->PulsePin = new PwmPin(pwmPinName, -1.0, 0.0);

  /*Set parameters*/
  this->SetRampSpeed(startingRampSpeed);
  this->SetVelocity(startingVelocity);
}

VHN2Motor::~VHN2Motor()
{
  delete this->PinA;
  delete this->PinB;
  delete this->PulsePin;
}

double VHN2Motor::GetVelocity()
{
  double velocityMag = this->PulsePin->GetValue();

  /*TODO: Determine directional relationships*/
  if (this->PinA->GetValue() == Clank::Hardware::GpioValue::High)
  {
    return velocityMag;
  }
  return -1*velocityMag;
}

void VHN2Motor::SetVelocity(double velocity)
{
  if (velocity < -100.0 || velocity > 100.0)
  {
    string errorMessage = "VHN2 motor error: invalid velocity.\n";
    errorMessage += "Attempted to set velocity to an invalid value.\n";
    errorMessage += "Expected float on the range [-100.0, 100.0], got ";
    errorMessage += to_string(velocity);
    throw runtime_error(errorMessage);
  }

  if (velocity < 0)
  {
    this->PinA->SetValue(Clank::Hardware::GpioValue::Low);
    this->PinB->SetValue(Clank::Hardware::GpioValue::High);
    velocity *= -1;
  }
  else if (velocity > 0)
  {
    this->PinA->SetValue(Clank::Hardware::GpioValue::High);
    this->PinB->SetValue(Clank::Hardware::GpioValue::Low);
  }
  else
  {
    this->PinA->SetValue(Clank::Hardware::GpioValue::Low);
    this->PinB->SetValue(Clank::Hardware::GpioValue::Low);
  }

  this->PulsePin->SetValue(velocity);
  return;
}

double VHN2Motor::GetRampSpeed()
{
  return this->PulsePin->GetRampSpeed();
}

void VHN2Motor::SetRampSpeed(double speed)
{
  if (speed == 0)
  {
    string errorMessage = "VHN2 motor error: invalid ramp speed.\n";
    errorMessage += "Attempted to set ramp speed to an invalid value.\n";
    errorMessage += "Expected non-zero integer, got ";
    errorMessage += to_string(speed);
    throw runtime_error(errorMessage);
  }

  this->PulsePin->SetRampSpeed(speed);
}

