#ifndef SAMPERIPHERAL_HPP
#define SAMPERIPHERAL_HPP

#include <string>

#include "configuration/Configuration.hpp"
#include "SamSerialPort.hpp"
#include "SamPeripheralManager.hpp"
#include "SamPeripheralExtentions.hpp"

namespace Clank
{
  namespace Hardware
  {
    /*Forward declarations for compiler*/
    class SamPeripheralManager;
    class SamSerialPort;

    class SamPeripheral
    {
      public:
        SamPeripheral(Clank::Hardware::SamPeripheralName name);
        SamPeripheral(std::string name);
        virtual ~SamPeripheral();

        virtual void ParseMessage(std::string input) = 0;

        virtual bool IsConsistant() = 0;

      protected:

        void SetState();
        Clank::Hardware::SamPeripheralName GetName();
        virtual std::string StateToMessage() = 0;

      private:
        void Init(Clank::Hardware::SamPeripheralName name);

        Clank::Hardware::SamSerialPort *SamSerialPort;
        Clank::Hardware::SamPeripheralManager *SamPeripheralManager;
        Clank::Hardware::SamPeripheralName Name;
    };
  }
}

#endif
