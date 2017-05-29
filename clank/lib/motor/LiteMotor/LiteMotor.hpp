#ifndef LITEMOTOR_HPP
#define LITEMOTOR_HPP

#include "motor/IMotor.hpp"
#include "configuration/Configuration.hpp"
#include "hardware/GpioPin.hpp"
#include "hardware/PwmPin.hpp"

#include <string>
#include <stdexcept>

namespace Clank
{
  namespace Motor
  {
    class LiteMotor : IMotor
    {
      public:
        LiteMotor(Clank::Configuration::Configuration *configuration);
        LiteMotor(double startingRampSpeed, double startingVelocity, std::string dirControlName, std::string pwmPinName);
        ~LiteMotor();

        double GetVelocity();
        void SetVelocity(double velocity);

        double GetRampSpeed();
        void SetRampSpeed(double speed);

      private:
        Clank::Hardware::GpioPin *DirPin;

        Clank::Hardware::PwmPin *PulsePin;
    };
  }
}

#endif
