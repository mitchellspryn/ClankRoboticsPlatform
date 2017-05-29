#include "Watchdog.hpp"

using namespace std;
using namespace Clank::Configuration;
using namespace Clank::Timers;

/*TODO: Can these be moved to class members?*/
void* WatchdogThreadFxn(void* arg)
{
  struct timespec time_spec;
  struct timeval time_val;
  Watchdog* dog = (Watchdog*)arg;
  pthread_cond_t* condition = dog->GetConditional();
  pthread_mutex_t* mutex = dog->GetMutex();
  
  while(dog->ShouldKeepRunning())
  {
    /*Get the current time*/
    gettimeofday(&time_val, NULL);
    time_spec.tv_nsec = time_val.tv_usec * 1000;
    time_spec.tv_sec = time_val.tv_sec;

    /*Add the timeout*/
    int msTimeout = dog->GetTimeoutMillis();
    unsigned long long newMillis = ((unsigned long long)msTimeout * 1000000) + (unsigned long long) time_spec.tv_nsec;

    time_spec.tv_nsec = newMillis % 1000000000;
    time_spec.tv_sec = time_spec.tv_sec + newMillis / 1000000000;

    /*Wait for timeout or kick*/
    /*pthread_cond_timedwait expects a LOCKED mutex*/
    /*It will release it once the thread blocks*/
    pthread_mutex_lock(mutex);

    int returnCode = pthread_cond_timedwait(condition, mutex, &time_spec);

    if (returnCode == ETIMEDOUT && dog->ShouldKeepRunning())
    {
      dog->Alert(); 
    }

    pthread_mutex_unlock(mutex);
  }

  pthread_exit(NULL);
}

Watchdog::Watchdog(Clank::Configuration::Configuration* configuration)
{
  string startingTimeoutMs;
  if (!configuration->GetConfigurationData("Timeout", startingTimeoutMs))
  {
    string message = "Watchdog Error: Invalid configuration for watchdog.\n";
    message += "Configuration: <Timeout>";
    throw runtime_error(message);
  }

  int timeoutMs = stoi(startingTimeoutMs);
  if (timeoutMs == 0)
  {
    string message = "Watchdog Error: Invalid configuration for watchdog.\n";
    message += "Expected integer for Timeout, received ";
    message += startingTimeoutMs;
    message += "\n";
    throw runtime_error(message);
  }

  this->TimeoutMs = timeoutMs;
  this->KeepRunning = false;
  this->TimeoutListeners = vector<function<void(void)> >();
}

Watchdog::~Watchdog()
{
  this->Stop();
}

void Watchdog::SetTimeoutMillis(long millis)
{
  if (millis <= 0)
  {
    string message = "Watchdog error: Attempting to set watchdog to a timeout <= 0.\n";
    message += "Attempted to set timeout to: ";
    message += to_string(millis);
    throw runtime_error(message);
  }

  if (this->KeepRunning)
  {
    pthread_mutex_lock(&WatchdogThreadMutex);
  }

  this->TimeoutMs = millis;

  if (this->KeepRunning)
  {
    pthread_mutex_unlock(&WatchdogThreadMutex);
  }
}

long Watchdog::GetTimeoutMillis()
{
  long returnMs;
  if (this->KeepRunning)
  {
    pthread_mutex_lock(&WatchdogThreadMutex);
  }

  returnMs =  this->TimeoutMs;

  if (this->KeepRunning)
  {
    pthread_mutex_unlock(&WatchdogThreadMutex);
  }

  return returnMs;
}

bool Watchdog::Start()
{
  this->KeepRunning = true;
  pthread_mutex_init(&WatchdogThreadMutex, NULL);
  pthread_cond_init(&WatchdogConditional, NULL);
  pthread_create(&WatchdogThread, NULL, WatchdogThreadFxn, this);
  return true;
}

void Watchdog::Kick()
{
  if (this->KeepRunning)
  {
    pthread_mutex_lock(&WatchdogThreadMutex);
    pthread_cond_signal(&WatchdogConditional);
    pthread_mutex_unlock(&WatchdogThreadMutex);
  }
}

bool Watchdog::Stop()
{
  if (this->KeepRunning)
  {
    pthread_mutex_lock(&WatchdogThreadMutex);
    this->KeepRunning = false;
    pthread_cond_signal(&WatchdogConditional);
    pthread_mutex_unlock(&WatchdogThreadMutex);

    pthread_join(WatchdogThread, NULL);
    pthread_cond_destroy(&WatchdogConditional);
    pthread_mutex_destroy(&WatchdogThreadMutex);
  }
  return true;
}

void Watchdog::AddTimeoutCallback(function<void(void)> target)
{
  this->TimeoutListeners.push_back(target);
}

bool Watchdog::ShouldKeepRunning()
{
  return this->KeepRunning;
}

pthread_cond_t* Watchdog::GetConditional()
{
  return &WatchdogConditional;
}

pthread_mutex_t* Watchdog::GetMutex()
{
  return &WatchdogThreadMutex;
}

void Watchdog::Alert()
{
  for (unsigned int i = 0; i< this->TimeoutListeners.size(); i++)
  {
    this->TimeoutListeners[i]();
  }
}
