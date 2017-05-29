#ifndef SAMCONTROLLER_HPP
#define SAMCONTROLLER_HPP

#include "hardware/SamPeripheralManager.hpp"

#include <mutex>
#include <string>
#include <fstream>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 2048


namespace Clank
{
  namespace Hardware
  {
    /*Forward declarations for compiler*/
    class SamPeripheralManager;

    class SamSerialPort
    {
      public:
        /*Singleton pattern*/
        static Clank::Hardware::SamSerialPort* Instance();

        /*Sends message and gets response*/
        void SendMessage(std::string message);
        bool IsListening();
        int GetSerialPortFd();
        void ParseMessage(std::string message);
        
        
      private:
        SamSerialPort();
        ~SamSerialPort();

        Clank::Hardware::SamPeripheralManager* SamPeripheralManager;

        static Clank::Hardware::SamSerialPort* SamSerialPortSingletonInstance;

        bool isListening;

        int SerialPortFd;

        pthread_t serialPortThread;
        pthread_mutex_t serialPortThreadMutex;
    };
  }
}

#endif
