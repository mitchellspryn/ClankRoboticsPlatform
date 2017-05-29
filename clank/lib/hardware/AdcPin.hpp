#ifndef ADCPIN_HPP
#define ADCPIN_HPP

#include "configuration/Configuration.hpp"
#include "hardware/SamPeripheralExtentions.hpp"
#include "hardware/SamPeripheral.hpp"

#include <string>
#include <sstream>
#include <stdexcept>

namespace Clank
{
  namespace Hardware
  {
    class AdcPin: SamPeripheral
    {
      public:
        AdcPin(std::string name, Clank::Configuration::Configuration* config);
        AdcPin(Clank::Hardware::SamPeripheralName name, Clank::Configuration::Configuration* config);
        AdcPin(Clank::Hardware::SamPeripheralName name, double threshold);
        AdcPin(std::string name, double threshold);

        ~AdcPin();

        void SetDeltaThreshold(double threshold);
        double GetDeltaThreshold();

        /*Returns a value between 0.0 and 1.0, linearly mapped from 0.0V -> 3.3V*/
        double GetValue();

        bool IsConsistant();

      protected:
        std::string StateToMessage();
        void ParseMessage(std::string message);

      private:
        double LastReportedValue;
        double LastReportedDeltaThreshold;
        double DeltaThresholdSetPoint;

    };
  }
}

#endif
