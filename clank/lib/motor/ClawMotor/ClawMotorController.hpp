#ifndef CLAWMOTORCONTROLLER_HPP
#define CLAWMOTORCONTROLLER_HPP

#include "../IMotor.hpp"
#include "configuration/Configuration.hpp"
#include "ClawMotor.hpp"

#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <errno.h>
#include <termios.h>
#include <limits.h>

#include <pthread.h>

#include <string>
#include <stdexcept>
#include <cmath>

#define CLAW_MOTOR_BUF_SIZE 16 //maximum number of bytes in a single command

namespace Clank
{
  namespace Motor
  {
    class ClawMotorController
    {
      public:
        ClawMotorController(Clank::Configuration::Configuration *configuration);
        ClawMotorController(std::string serialPortName, unsigned int baudRate, unsigned int timeoutMillis, unsigned int address);

        ~ClawMotorController();

        Clank::Motor::IMotor* GetMotor1();
        Clank::Motor::IMotor* GetMotor2();

      private:
        unsigned char DataBuffer[CLAW_MOTOR_BUF_SIZE];
        struct timeval Timeout;
        fd_set FileDescriptorSet;
        unsigned int Address;

        double Motor1Velocity;
        double Motor1RampSpeed;
        double Motor2Velocity;
        double Motor2RampSpeed;

        void Initialize(std::string serialPortName, unsigned int baudRate, unsigned int timeoutMIllis, unsigned int address);

        double GetMotor1Velocity();
        void SetMotor1Velocity(double velocity);

        double GetMotor1RampSpeed();
        void SetMotor1RampSpeed(double speed);

        double GetMotor2Velocity();
        void SetMotor2Velocity(double velocity);

        double GetMotor2RampSpeed();
        void SetMotor2RampSpeed(double speed);

        bool WriteAndVerify(unsigned int numBytes);

        unsigned int ComputeCRC(unsigned int numBytes);

        int SerialPortFd;

        Clank::Motor::ClawMotor* Motor1;
        Clank::Motor::ClawMotor* Motor2;

        pthread_mutex_t mutex;
        
    };
  }
}

#endif
