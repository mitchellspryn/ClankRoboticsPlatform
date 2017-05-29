#include <hardware/PwmPin.hpp>
#include <configuration/Configuration.hpp>

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;
using namespace Clank::Hardware;
using namespace Clank::Configuration;

bool stopThread = false;

const double defaultRamp = -1.0;
const double defaultValue = 0.0;

pthread_t threadType;
pthread_mutex_t threadMutexType;

void PrintPwmState(double rampSpeed, double value)
{
  cout << "The PWM now has a value of <" << value << "> (speed = " << rampSpeed << ")" << endl;
  return;
}

void* ReadThread(void* arg)
{
  PwmPin *pin = (PwmPin*)arg;
  pthread_mutex_lock(&threadMutexType);
  double pastSpeed = pin->GetRampSpeed();
  double pastValue = pin->GetValue();
  pthread_mutex_unlock(&threadMutexType);

  while(!stopThread)
  {
    pthread_mutex_lock(&threadMutexType);
    double newSpeed = pin->GetRampSpeed();
    double newValue = pin->GetValue();
    if (newSpeed != pastSpeed || newValue != pastValue)
    {
      pastValue = newValue;
      pastSpeed = newSpeed;
      PrintPwmState(newSpeed, newValue);
      if (!pin->IsConsistant())
      {
        cout << "Pin not consistant..." << endl;
      }
    }
    pthread_mutex_unlock(&threadMutexType);

    usleep(50000);
  }
  return NULL;
}

void EatNewline()
{
  char c = getchar();
  while(c != '\n')
    c = getchar();
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cout << "Usage: ./PwmTest <PinNumber>" << endl;
    cout << "  <PinNumber> - PWM pin to manipulate" << endl;
    cout << "enter 'r' to set the ramp speed (<0 -> instant change, 0 = undefined)" << endl;
    cout << "  the default value for the ramp speed is " << to_string(defaultRamp) << endl;
    cout << "enter 'v' to change the value (on range [0.0, 100.0])" << endl;
    cout << "  the default value for the speed is " << to_string(defaultValue) << endl;
    return 0;
  }

  cout << "Creating starting configuration..." << endl;
  Configuration *config = new Configuration();
  config->AddConfigurationData("RampSpeed", to_string(defaultRamp));
  config->AddConfigurationData("Value", to_string(defaultValue));

  string pwmName_str = "PWM" + string(argv[1]);

  cout << "Attempting to register PWM " << pwmName_str << endl;

  PwmPin *pin = new PwmPin(pwmName_str, config);
  usleep(100000);
  delete config;

  cout << "Starting read thread..." << endl;
  pthread_mutex_init(&threadMutexType, NULL);
  pthread_create(&threadType, NULL, ReadThread, pin);

  cout << "Done!" << endl;
  cout << "enter 'r' to set the ramp speed (<0 -> instant change, 0 = undefined)" << endl;
  cout << "  the default value for the ramp speed is " << to_string(defaultRamp) << endl;
  cout << "enter 'v' to change the value (on range [0.0, 100.0])" << endl;
  cout << "  the default value for the speed is " << to_string(defaultValue) << endl;

  pthread_mutex_lock(&threadMutexType);
  cout << "Printing initial state..." << endl;
  PrintPwmState(pin->GetRampSpeed(), pin->GetValue());
  if (!pin->IsConsistant())
  {
    cout << "Pin is not consistant..." << endl;
  }
  else
  {
    cout << "Pin is consistant." << endl;
  }
  pthread_mutex_unlock(&threadMutexType);
  
  char cmd = 'w';
  while(cmd != 'q')
  {
    cmd = getchar();
    if (cmd == 'r')
    {
      pthread_mutex_lock(&threadMutexType);
      string input = "w";
      double newSpeed = 0.0;
      do
      {
        cout << "Enter new ramp speed: ";
        EatNewline();
        getline(cin, input);
        newSpeed = stod(input);
        if (newSpeed == 0 && !input.compare("q"))
        {
          cout << "Invalid input. Try again or enter 'q' to quit." << endl; 
        }
        else
        {
          if (!input.compare("q"))
          {
            cmd = 'q';
          }
          else
          {
            pin->SetRampSpeed(newSpeed);
          }
        }
      } while(newSpeed == 0 && !input.compare("q"));
      pthread_mutex_unlock(&threadMutexType);
    }

    else if (cmd == 'v')
    {
      pthread_mutex_lock(&threadMutexType);
      string input = "w";
      double newValue = 0.0;

      /*TODO: Method of validating input?*/
      cout << "Enter new value: ";
      EatNewline();
      getline(cin, input);
      newValue = stod(input);
      
      if (!input.compare("q"))
      {
        cmd = 'q';
      }
      else
      {
        pin->SetValue(newValue);
      }
      
      pthread_mutex_unlock(&threadMutexType);
    }

    else if (cmd != '\n' && cmd != 'q')
    {
      cout << "Unrecognized command." << endl;
    }
  }

  cout << "Stopping background thread..." << endl;

  stopThread = true;
  pthread_cancel(threadType);
  pthread_join(threadType, NULL);
  pthread_mutex_destroy(&threadMutexType);

  cout << "Program terminating..." << endl;
  return 0;
}
