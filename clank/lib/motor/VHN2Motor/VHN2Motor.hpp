#ifndef VHN2MOTOR_HPP
#define VHN2MOTOR_HPP

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
    class VHN2Motor : IMotor
    {
      public:
        VHN2Motor(Clank::Configuration::Configuration *configuration);
        VHN2Motor(double startingRampSpeed, double startingVelocity, std::string gpioAName, std::string gpioBName, std::string pwmPinName);
        ~VHN2Motor();

        double GetVelocity();
        void SetVelocity(double velocity);
        
        double GetRampSpeed();
        void SetRampSpeed(double speed);

      private:
        Clank::Hardware::GpioPin *PinA;
        Clank::Hardware::GpioPin *PinB;

        Clank::Hardware::PwmPin *PulsePin;
    };
  }
}

#endif
