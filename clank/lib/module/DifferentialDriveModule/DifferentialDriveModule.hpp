#ifndef DIFFERENTIALDRIVESYSTEM_HPP
#define DIFFERENTIALDRIVESYSTEM_HPP

#include "motor/IMotor.hpp"
#include "motor/ClawMotor/ClawMotorController.hpp"
#include "configuration/Configuration.hpp"

#include <string>
#include <cmath>

namespace Clank
{
  namespace Module
  {
    class DifferentialDriveModule
    {
      public:
        DifferentialDriveModule(Clank::Configuration::Configuration *configuration);
        ~DifferentialDriveModule();

        double GetSpeed();
        double GetHeading();
        void Stop();

        bool Drive(double speed, double heading);

        void SetRampSpeed(double rampSpeed);
        double GetRampSpeed();

      private:
        Clank::Motor::ClawMotorController* MotorController;
        Clank::Motor::IMotor* RightMotor;
        Clank::Motor::IMotor* LeftMotor;

        double SetSpeed;
        double SetHeading;

        bool ValidateInput(double speed, double heading);

        /*Set the pins to drive the robot according to the member variables 
         * SetSpeed and SetHeading*/
        void SetHardware();

        /*Computes a smooth curve between 0 and pi/2*/
        double GetCosineMultiplier(double value);
    };
  }
}

#endif
