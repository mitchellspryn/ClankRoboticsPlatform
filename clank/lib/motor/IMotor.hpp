#ifndef IMOTOR_HPP
#define IMOTOR_HPP

#include "configuration/Configuration.hpp"
#include "hardware/GpioPin.hpp"
#include "hardware/PwmPin.hpp"

#include <string>

namespace Clank
{
  namespace Motor
  {
    class IMotor
    {
      public:
        virtual ~IMotor() {;};

        virtual double GetVelocity() = 0;
        virtual void SetVelocity(double velocity) = 0;
        
        virtual double GetRampSpeed() = 0;
        virtual void SetRampSpeed(double speed) = 0;

    };
  }
}

#endif
