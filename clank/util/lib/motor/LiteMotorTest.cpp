#include "motor/LiteMotor/LiteMotor.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace Clank::Motor;

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cout << "Usage: $./LiteMotorTester <PwmPinNumber> <DirPinNumber>" << endl;
    cout << "\t<PwmPinNumber>: The PWM pin to use for speed control" << endl;
    cout << "\t<DirPinNumber>: The directional control pin" << endl;
    return 0;
  }

  string dirName = "GPIO" + string(argv[0]);
  string pwmName = "PWM" + string(argv[1]);

  cout << "Creating motor with Dir pin " << dirName << "and PWM pin " << pwmName << "..." << endl;

  LiteMotor* motor = new LiteMotor(-1.0, 0.0, dirName, pwmName);

  cout << "Done! " << endl;
  cout << "Enter 's' to change the speed of the motor." << endl;
  cout << "Enter 'r' to change the ramp speed of the motor." << endl;
  cout << "Enter 'q' to quit." << endl;

  bool done = false;
  char cmd;
  while(!done)
  {
    cout << "Enter a command: ";
    cmd = getchar();
    switch (cmd)
    {
      case 's':
      {
        cout << "Enter the new speed (number from -100 to 100): ";
        string userInput;
        getline(cin, userInput);
        double speed = stod(userInput);
        if (speed < -100 || speed > 100)
        {
          cout << "Invalid input. Value out of range." << endl;
        }
        else
        {
          cout << "Setting the motor to " << to_string(speed) << endl;
          motor->SetVelocity(speed);
        }
        break;
      }
      case 'r':
      {
        cout << "Enter the new ramp speed (number > 0 or -1): ";
        string userInput;
        getline(cin, userInput);
        double rSpeed = stod(userInput);
        if (rSpeed <= 0 && rSpeed != -1)
        {
          cout << "Invalid input. Value out of range." << endl;
        }
        else
        {
          cout << "Setting ramp speed to " << to_string(rSpeed) << endl;
          motor->SetRampSpeed(rSpeed);
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
        if (cmd != '\n')
        {
          cout << "Unrecognized command." << endl;
        }
        break;
      }
    }
  }

  cout << "Cleaning up..." << endl;
  delete motor;
  cout << "Graceful termination." << endl;
  return 0;
}
