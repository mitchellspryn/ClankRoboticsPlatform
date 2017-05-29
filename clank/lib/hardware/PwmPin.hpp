#ifndef PWMPIN_HPP
#define PWMPIN_HPP

#include "configuration/Configuration.hpp"
#include "hardware/SamPeripheralExtentions.hpp"
#include "hardware/SamPeripheral.hpp"

#include <string>
#include <stdexcept>
#include <sstream>

namespace Clank
{
  namespace Hardware
  {
    class PwmPin: SamPeripheral
    {
      public:

        PwmPin(std::string name, Clank::Configuration::Configuration *config);
        PwmPin(Clank::Hardware::SamPeripheralName name, Clank::Configuration::Configuration* config);
        PwmPin(Clank::Hardware::SamPeripheralName, double rampSpeed, double value);
        PwmPin(std::string name, double rampSpeed, double value);

        ~PwmPin();

        void SetRampSpeed(double speed);
        double GetRampSpeed();

        void SetValue(double value);
        double GetValue();

        bool IsConsistant();

      protected:
        std::string StateToMessage();
        void ParseMessage(std::string message);

      private:
        double StagingRampSpeed;
        double LastReportedRampSpeed;

        double StagingValue;
        double LastReportedValue;
    };
  }
}

#endif
