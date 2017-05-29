#ifndef SAMPERIPHERALMANAGER_HPP
#define SAMPERIPHERALMANAGER_HPP

#include <string>
#include <stdexcept>
#include <map>
#include "hardware/SamPeripheralExtentions.hpp"
#include "hardware/SamSerialPort.hpp"
#include "hardware/SamPeripheral.hpp"

namespace Clank
{
  namespace Hardware
  {
    /*Forward declarations for compiler*/
    class SamPeripheral;
    class SamSerialPort;

    class SamPeripheralManager
    {
      public:
        static Clank::Hardware::SamPeripheralManager* Instance();
        void Get(Clank::Hardware::SamPeripheralName name, Clank::Hardware::SamPeripheral* peripheralInstance);
        void Return(Clank::Hardware::SamPeripheralName name);
        void RouteMessage(std::string message);
        void InitializeSerialPort();

      private:

        struct RequestType
        {
          bool RequestState;
          bool ActualState;
          bool UpdatedByHardware;
        };

        SamPeripheralManager();
        static Clank::Hardware::SamPeripheralManager *SamPeripheralManagerSingletonInstance;
        std::map<Clank::Hardware::SamPeripheralName, Clank::Hardware::SamPeripheral*> PeripheralMap;
        Clank::Hardware::SamSerialPort *SamSerialPort;
        std::map<Clank::Hardware::SamPeripheralName, RequestType> EnabledStates;
    };
  }
}

#endif
