#include "robot/TeleoperatedRobot.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace Clank::Robot;

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cout << "Usage: ./TeleoperatedRobot <ConfigFile>" << endl;
    cout << "  <ConfigFile> : Configuration file for robot." << endl;
    return 0;
  }

  string configFileName = argv[1];

  cout << "Initializing robot from " << configFileName << endl;

  TeleoperatedRobot* robot = new TeleoperatedRobot();
  robot->ConstructFromFile(configFileName);

  cout << "Done! Press enter to quit." << endl;

  getchar();

  cout << "Deleting robot..." << endl;

  delete robot;

  cout << "Done! Terminating successfully." << endl;
  return 0;
}
