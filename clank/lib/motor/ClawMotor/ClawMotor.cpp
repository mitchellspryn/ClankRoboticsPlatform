#include "ClawMotor.hpp"

using namespace std;
using namespace Clank::Motor;

ClawMotor::ClawMotor
(
  function<double(void)> GetVelocityFxn,
  function<void(double)> SetVelocityFxn,
  function<double(void)> GetRampSpeedFxn,
  function<void(double)> SetRampSpeedFxn
)
{
  this->GetVelocityFunction = GetVelocityFxn;
  this->SetVelocityFunction = SetVelocityFxn;
  this->GetRampSpeedFunction = GetRampSpeedFxn;
  this->SetRampSpeedFunction = SetRampSpeedFxn;
}

ClawMotor::~ClawMotor()
{
  /*TODO: Do we need to clean up references?*/
}

double ClawMotor::GetVelocity()
{
  return this->GetVelocityFunction();
}

void ClawMotor::SetVelocity(double velocity)
{
  this->SetVelocityFunction(velocity);
}

double ClawMotor::GetRampSpeed()
{
  return this->GetRampSpeedFunction();
}

void ClawMotor::SetRampSpeed(double speed)
{
  this->SetRampSpeedFunction(speed);
}
