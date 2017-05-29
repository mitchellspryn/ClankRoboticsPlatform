#include "motor/ClawMotor/ClawMotorController.hpp"
#include "motor/IMotor.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace Clank::Motor;

int main(int argc, char** argv)
{
  if (argc != 5)
  {
    cout << "Usage: $./RoboClawMotorTester <SerialPortName> <BaudRate> <TimeoutMillis> <Address>" << endl;
    cout << "\t<SerialPortName> : The full path name for the controller (i.e. '/dev/ttyACM0')" << endl;
    cout << "\t<BaudRate> : The communication baud rate" << endl;
    cout << "\t<TimeoutMillis> : The communication timeout, in milliseconds." << endl;
    cout << "\t<Address> : The motor controller address." << endl;
    return 0;
  }

  cout << "Initializing..." << endl;

  string serialPortName = argv[1];
  unsigned int baudRate = (unsigned int)stoi(argv[2]);
  unsigned int timeoutMillis = (unsigned int)stoi(argv[3]);
  unsigned int address = (unsigned int)stoi(argv[4]);
  
  ClawMotorController *controller = new ClawMotorController(serialPortName, baudRate, timeoutMillis, address);

  IMotor* motor1 = controller->GetMotor1();
  IMotor* motor2 = controller->GetMotor2();

  cout << "Done! " << endl;
  
  bool done = false;
  while(!done)
  {
    cout << "Enter 's' to change the speed of one of the motors." << endl;
    cout << "Enter 'r' to change the ramp speed of one of the motors." << endl;
    cout << "Enter 'q' to quit." << endl;
    cout << "Enter a command: ";
    string userInput;
    getline(cin, userInput);
    switch (userInput[0])
    {
      case 's':
      {
        cout << "Which motor (1 or 2)? ";
        getline(cin, userInput);
        bool isMotor1 = false;
        bool isValid = true;
        if (userInput[0] == '1')
        {
          isMotor1 = true;
        }
        else if (userInput[0] != '2')
        {
          cout << "Invalid input. " << endl;
          isValid = false;
        }

        if (isValid)
        {
          cout << "Enter the new speed (number from -100 to 100): ";
          getline(cin, userInput);
          double speed = stod(userInput);
          if (speed < -100 || speed > 100)
          {
            cout << "Invalid input. Value out of range." << endl;
          }
          else
          {
            if (isMotor1)
            {
              cout << "Setting motor 1 to " << to_string(speed) << endl;
              motor1->SetVelocity(speed);
            }
            else
            {
              cout << "Setting motor 2 to " << to_string(speed) << endl;
              motor2->SetVelocity(speed);
            }
          }
        }
        break;
      }
      case 'r':
      {
        cout << "Which motor (1 or 2)? ";
        getline(cin, userInput);
        bool isMotor1 = false;
        bool isValid = true;
        if (userInput[0] == '1')
        {
          isMotor1 = true;
        }
        else if (userInput[0] != '2')
        {
          cout << "Invalid input. " << endl;
          isValid = false;
        }

        if (isValid)
        {
          cout << "Enter the new ramp speed (number > 0 or -1): ";
          getline(cin, userInput);
          double rSpeed = stod(userInput);
          if (rSpeed <= 0 && rSpeed != -1)
          {
            cout << "Invalid input. Value out of range." << endl;
          }
          else
          {
            if (isMotor1)
            {
              cout << "Setting ramp speed for motor 1 to " << to_string(rSpeed) << endl;
              motor1->SetRampSpeed(rSpeed);
            }
            else
            {
              cout << "Setting ramp speed for motor 2 to " << to_string(rSpeed) << endl;
              motor2->SetRampSpeed(rSpeed);
            }
            
          }
        }
        break;
      }
      case 'q':
      {
        done = true;
        break;
      }
      default:
      {
        cout << "Unrecognized command." << endl;
        break;
      }
    }
  }

  cout << "Cleaning up..." << endl;
  delete controller;
  cout << "Graceful termination." << endl;
  return 0;
}
