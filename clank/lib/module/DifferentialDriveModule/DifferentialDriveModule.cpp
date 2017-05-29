#include "DifferentialDriveModule.hpp"

using namespace std;
using namespace Clank::Configuration;
using namespace Clank::Motor;
using namespace Clank::Module;

DifferentialDriveModule::DifferentialDriveModule(Clank::Configuration::Configuration *configuration)
{
  /*Read and validate input*/
  Clank::Configuration::Configuration clawControllerConfig;
  if (!configuration->FindConfigurationByTag("ClawController", clawControllerConfig))
  {
    string errorMessage = "DifferentialDriveModule error: no config for Claw Motor Controller\n";
    errorMessage += "\tExpected subconfiguration with tag <ClawController>.";
    throw runtime_error(errorMessage);
  }

  /*Create the motors*/
  this->MotorController = new ClawMotorController(&clawControllerConfig);
  this->RightMotor = this->MotorController->GetMotor1();
  this->LeftMotor = this->MotorController->GetMotor2();

  this->Stop(); 
}

DifferentialDriveModule::~DifferentialDriveModule()
{

}

double DifferentialDriveModule::GetSpeed()
{
  return this->SetSpeed;
}

double DifferentialDriveModule::GetHeading()
{
  return this->SetHeading;
}

void DifferentialDriveModule::Stop()
{
  this->SetSpeed = 0;
  this->SetHeading = 0;
  this->SetHardware();
}

bool DifferentialDriveModule::Drive(double speed, double heading)
{
  if (this->ValidateInput(speed, heading))
  {
    this->SetSpeed = speed;
    this->SetHeading = heading;
    this->SetHardware();
    return true;
  }
  return false;
}

void DifferentialDriveModule::SetRampSpeed(double rampSpeed)
{
  /*TODO: validate?*/
  this->RightMotor->SetRampSpeed(rampSpeed);
  this->LeftMotor->SetRampSpeed(rampSpeed);
  return;
}

double DifferentialDriveModule::GetRampSpeed()
{
  double sp1 = this->RightMotor->GetRampSpeed();
  double sp2 = this->LeftMotor->GetRampSpeed();

  if (sp1 == sp2)
    return sp1;

  string errorMessage = "DifferentialDriveModule Runtime exception:\n";
  errorMessage += "Left and right ramp speeds not equal.";
  errorMessage += "Left = ";
  errorMessage += to_string(sp2);
  errorMessage += "\nRight = ";
  errorMessage += to_string(sp1);
  throw runtime_error(errorMessage);
}

bool DifferentialDriveModule::ValidateInput(double speed, double heading)
{
  return ((speed >= 0.0 && speed <= 100.0)
          &&
          (heading >= 0.0 && heading <= 2*M_PI));
}

void DifferentialDriveModule::SetHardware()
{
  double leftMotorVelocity, rightMotorVelocity;  
  
  if (this->SetHeading < M_PI/2.0)
  {
    leftMotorVelocity = this->GetCosineMultiplier(this->SetHeading) * this->SetSpeed;
    rightMotorVelocity = this->SetSpeed;
  }
  else if (this->SetHeading < M_PI)
  {
    leftMotorVelocity = -1.0 * this->SetSpeed;
    rightMotorVelocity = this->GetCosineMultiplier(this->SetHeading - (M_PI/2.0)) * this->SetSpeed;
  }
  else if (this->SetHeading < ((3.0 * M_PI) / 2.0))
  {
    leftMotorVelocity = (-1.0 * this->GetCosineMultiplier(this->SetHeading - (M_PI))) * this->SetSpeed;
    rightMotorVelocity = -1.0 * this->SetSpeed;
  }
  else
  {
    leftMotorVelocity = this->SetSpeed;
    rightMotorVelocity = (-1.0 * this->GetCosineMultiplier(this->SetHeading - (M_PI * 3.0 / 2.0))) * this->SetSpeed;
  }

  this->LeftMotor->SetVelocity(leftMotorVelocity);
  this->RightMotor->SetVelocity(rightMotorVelocity);
}

/*Maps 0 -> 1; pi/2 -> -1*/
double DifferentialDriveModule::GetCosineMultiplier(double value)
{
  return cos(2 * value);
}
