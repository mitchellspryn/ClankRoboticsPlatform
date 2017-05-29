#include <hardware/GpioPin.hpp>
#include <hardware/GpioTypes.hpp>
#include <configuration/Configuration.hpp>

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;
using namespace Clank::Hardware;
using namespace Clank::Configuration;

bool stopThread = false;

pthread_t threadType;
pthread_mutex_t threadMutexType;

void PrintPinState(GpioDirection dir, GpioValue val)
{
  cout << "The pin now has a direction of ";
  if (dir == Input)
    cout << "Input ";
  else if (dir == Output)
    cout << "Output ";
  else
    cout << "Unknown";

  cout << "and a value of ";
  if (val == High)
    cout << "High";
  else if (val == Low)
    cout << "Low";

  cout << endl;
}

void* ReadThread (void* arg)
{
  GpioPin *pin = (GpioPin*)arg;

  pthread_mutex_lock(&threadMutexType);
  GpioDirection pastDirection = pin->GetDirection();
  GpioValue pastValue = pin->GetValue();
  pthread_mutex_unlock(&threadMutexType);

  while(!stopThread)
  {
    pthread_mutex_lock(&threadMutexType);
    GpioDirection nextDirection = pin->GetDirection();
    GpioValue nextValue = pin->GetValue();
    pthread_mutex_unlock(&threadMutexType);

    if (nextDirection != pastDirection || nextValue != pastValue)
    {
      pastDirection = nextDirection;
      pastValue = nextValue;
      PrintPinState(nextDirection, nextValue);
      if (!pin->IsConsistant())
      {
        cout << "Pin is not consistant..." << endl;
      }
      else
      {
        cout << "Pin is consistant..." << endl;
      }
    }

    usleep(50000);
  }

  return NULL;
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cout << "Usage: ./GpioTest <PinNumber>" << endl;
    cout << "  <PinNumber> - GPIO pin to manipulate" << endl;
    cout << "enter 'h' to set the pin high, 'l' to set it low." << endl;
    cout << "enter 'o' to set the pin as an output, 'i' to set it as an input." << endl;
    cout << "the pin defaults to starting as an output, whose value is 'Low'." << endl;
    return 0;
  }

  cout << "Creating starting configuration..." << endl;
  Configuration *config = new Configuration();
  config->AddConfigurationData("Direction", "Out");
  config->AddConfigurationData("Value", "Low");

  string pinName_str = "GPIO" + string(argv[1]);

  cout << "Attempting to register pin " << pinName_str << endl;

  GpioPin *pin = new GpioPin(pinName_str, config);
  usleep(100000);

  delete config; //not needed any more, free the memory

  cout << "Starting read thread..." << endl;
  pthread_mutex_init(&threadMutexType, NULL);
  pthread_create(&threadType, NULL, ReadThread, pin);

  cout << "Done!" << endl;
  cout << "enter 'h' to set the pin high, 'l' to set it low." << endl;
  cout << "enter 'o' to set the pin as an output, 'i' to set it as an input." << endl;
  cout << "enter 'q' to quit." << endl;

  pthread_mutex_lock(&threadMutexType);
  cout << "Printing intitial state" << endl;
  PrintPinState(pin->GetDirection(), pin->GetValue());
  if (!pin->IsConsistant())
  {
    cout << "Pin is not consistant..." << endl;
  }
  else
  {
    cout << "Pin is consistant..." << endl;
  }
  pthread_mutex_unlock(&threadMutexType);

  char cmd = 'w';
  while(cmd != 'q')
  {
    cmd = getchar();
    if (cmd == 'o')
    {
      pthread_mutex_lock(&threadMutexType);
      pin->SetDirection(Output);
      pthread_mutex_unlock(&threadMutexType);
    }
    else if (cmd == 'i')
    {
      pthread_mutex_lock(&threadMutexType);
      pin->SetDirection(Input);
      pthread_mutex_unlock(&threadMutexType);
    }
    else if (cmd == 'h')
    {
      pthread_mutex_lock(&threadMutexType);
      pin->SetValue(High);
      pthread_mutex_unlock(&threadMutexType);
    }
    else if (cmd == 'l')
    {
      pthread_mutex_lock(&threadMutexType);
      pin->SetValue(Low);
      pthread_mutex_unlock(&threadMutexType);
    }
    else if (cmd != 'q' && cmd != '\n')
    {
      cout << "Unrecognized command." << endl;
    }
    usleep(10000);
    
  }

  cout << "Stopping background thread..." << endl;

  stopThread = true;
  pthread_cancel(threadType);
  pthread_join(threadType, NULL);
  pthread_mutex_destroy(&threadMutexType);

  cout << "Program terminating..." << endl;
  return 0;
}

