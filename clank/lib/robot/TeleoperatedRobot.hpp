#ifndef TELEOPERATEDROBOT_HPP
#define TELEOPERATEDROBOT_HPP

#include "configuration/Configuration.hpp"
#include "configuration/IndentedFileConfigurationReader.hpp"
#include "module/DifferentialDriveModule/DifferentialDriveModule.hpp"
#include "module/VideoStreamer/VideoStreamer.hpp"
#include "communication/TcpServer.hpp"
#include "communication/Command.hpp"
#include "timers/Watchdog.hpp"

#include <string>
#include <stdexcept>

namespace Clank
{
  namespace Robot
  {
    class TeleoperatedRobot
    {
      public:
        TeleoperatedRobot();
        ~TeleoperatedRobot();

        bool ConstructFromFile(std::string configurationFilePath);

      private:

        void OnWatchdogTimeout();
        const char* OnCommandReceived(Clank::Communication::Command* command);

        Clank::Module::DifferentialDriveModule *DifferentialDriveModule;
        Clank::Module::VideoStreamer *VideoStreamer;
        Clank::Communication::TcpServer *TcpServer;
        Clank::Timers::Watchdog *Watchdog;
    };
  }
}


#endif
