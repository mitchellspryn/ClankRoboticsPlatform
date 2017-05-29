#ifndef CLAWMOTOR_HPP
#define CLAWMOTOR_HPP

#include "motor/IMotor.hpp"
#include <functional>

namespace Clank
{
  namespace Motor
  {
    class ClawMotor : public IMotor
    {
      public:
        ClawMotor
        (
          std::function<double(void)> GetVelocityFxn,
          std::function<void(double)> SetVelocityFxn,
          std::function<double(void)> GetRampSpeedFxn,
          std::function<void(double)> SetRampSpeedFxn
        );

        ~ClawMotor();

        double GetVelocity();
        void SetVelocity(double velocity);

        double GetRampSpeed();
        void SetRampSpeed(double speed);

      private:
        std::function<double(void)> GetVelocityFunction;
        std::function<void(double)> SetVelocityFunction;
        std::function<double(void)> GetRampSpeedFunction;
        std::function<void(double)> SetRampSpeedFunction;
    };
  }
}




#endif
