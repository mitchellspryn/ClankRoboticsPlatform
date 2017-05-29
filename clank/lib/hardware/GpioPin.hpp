#ifndef GPIOPIN_HPP
#define GPIOPIN_HPP

#include "GpioTypes.hpp"
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
    class GpioPin : SamPeripheral
    {
      public:
        GpioPin(std::string name, Clank::Configuration::Configuration* config);
        GpioPin(Clank::Hardware::SamPeripheralName name, Clank::Configuration::Configuration* config);
        GpioPin(Clank::Hardware::SamPeripheralName name, Clank::Hardware::GpioDirection startingDirection, Clank::Hardware::GpioValue startingValue);
        GpioPin(std::string name, Clank::Hardware::GpioDirection startingDirection, Clank::Hardware::GpioValue startingValue);
                                                         
        ~GpioPin();

        void SetDirection(Clank::Hardware::GpioDirection direction);
        Clank::Hardware::GpioDirection GetDirection();

        void SetValue(Clank::Hardware::GpioValue value);
        Clank::Hardware::GpioValue GetValue();

        bool IsConsistant();

      protected:

        std::string StateToMessage();
        void ParseMessage(std::string message);


      private:
        Clank::Hardware::GpioDirection Direction;
        Clank::Hardware::GpioValue Value;

        Clank::Hardware::GpioDirection StagingDirection;
        Clank::Hardware::GpioValue StagingValue;

    };
  }
}

#endif
