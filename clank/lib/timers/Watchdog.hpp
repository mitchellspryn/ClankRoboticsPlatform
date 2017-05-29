#ifndef WATCHDOG_HPP
#define WATCHDOG_HPP

#include <pthread.h>

#include "configuration/Configuration.hpp"
#include <functional>
#include <vector>
#include <sys/time.h>
#include <errno.h>
#include <stdexcept>

namespace Clank
{
  namespace Timers
  {
    class Watchdog
    {
      public:
        Watchdog(Clank::Configuration::Configuration* configuration);
        ~Watchdog();

        void SetTimeoutMillis(long millis);
        long GetTimeoutMillis();

        bool Start();
        void Kick();
        bool Stop();

        void AddTimeoutCallback(std::function<void(void)> target);

        /*Internal functions used by thread*/
        bool ShouldKeepRunning();
        pthread_cond_t* GetConditional();
        pthread_mutex_t* GetMutex();
        void Alert();

      private:
        long TimeoutMs;
        std::vector<std::function<void(void)> > TimeoutListeners;
        pthread_t WatchdogThread;
        pthread_mutex_t WatchdogThreadMutex;
        pthread_cond_t WatchdogConditional;

        bool KeepRunning;
    };
  }
}

#endif
