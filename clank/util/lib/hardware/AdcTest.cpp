#include <hardware/AdcPin.hpp>
#include <configuration/Configuration.hpp>

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;
using namespace Clank::Hardware;
using namespace Clank::Configuration;

bool stopThread = false;

const double defaultThresh = 5.0;

pthread_t threadType;
pthread_mutex_t threadMutexType;

void PrintAdcState(double thresh, double val)
{
  cout << "The ADC now has a value of <" << val << "> (Threshold = " << thresh << ")" << endl;
  return;
}

void* ReadThread(void* arg)
{
  AdcPin *pin = (AdcPin*) arg;
  pthread_mutex_lock(&threadMutexType);
  double pastThresh = pin->GetDeltaThreshold();
  double pastVal = pin->GetValue();
  pthread_mutex_unlock(&threadMutexType);

  while(!stopThread)
  {
    pthread_mutex_lock(&threadMutexType);
    double newThresh = pin->GetDeltaThreshold();
    double newValue = pin->GetValue();
    if (newValue != pastVal || newThresh != pastThresh)
    {
      pastThresh = newThresh;
      pastVal = newValue;
      PrintAdcState(newThresh, newValue);
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
    cout << "Usage: ./AdcTest <PinNumber>" << endl;
    cout << "  <PinNumber> - ADC pin to manipulate" << endl;
    cout << "enter 't' to set the reporting threshold (between 0.0 and 100.0)" << endl;
    cout << "  the threshold defaults to " << defaultThresh << endl;
    return 0;
  }

  cout << "Creating starting configuration..." << endl;
  Configuration *config = new Configuration();
  config->AddConfigurationData("Threshold", to_string(defaultThresh));

  string adcName_str = "ADC" + string(argv[1]);

  cout << "Attempting to register ADC " << adcName_str << endl;

  AdcPin *pin = new AdcPin(adcName_str, config);
  usleep(100000);
  
  delete config;

  cout << "Starting read thread..." << endl;
  pthread_mutex_init(&threadMutexType, NULL);
  pthread_create(&threadType, NULL, ReadThread, pin);

  cout << "Done!" << endl;
  cout << "enter 't' to set the reporting threshold (between 0.0 and 100.0)" << endl;

  pthread_mutex_lock(&threadMutexType);
  cout << "Printing initial state..." << endl;
  PrintAdcState(pin->GetDeltaThreshold(), pin->GetValue());
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
    if (cmd == 't')
    {
      string input = "W";
      double newThresh = 0.0;

      /*Loop until user enters valid input or quits*/

      pthread_mutex_lock(&threadMutexType);
      while (!input.compare("q") || newThresh <= 0.0 || newThresh > 100.0)
      {
        cout << "Enter new thresohold value: ";
        EatNewline();
        getline(cin, input);
        newThresh = stod(input);
        if ( (newThresh <= 0.0 || newThresh > 100.0) && !input.compare("q"))
        {
          cout << "Invalid input. Try again or enter 'q' to quit." << endl;
        }

        if (!input.compare("q"))
        {
          cmd = 'q';
          break;
        }
        else
        {
          cout << "Setting threshold to " << newThresh << endl;
          pin->SetDeltaThreshold(newThresh); 
        }
      }
      pthread_mutex_unlock(&threadMutexType);
    }
    else if (cmd != '\n' && cmd != 'q')
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
