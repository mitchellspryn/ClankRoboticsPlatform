#include "timers/Watchdog.hpp"
#include "configuration/Configuration.hpp"
#include "gtest/gtest.h"
#include <unistd.h>
#include <iostream>

using namespace std;

int WatchdogExpireCount = 0;

void WatchdogCountFxn()
{
  WatchdogExpireCount++;
}

class WatchdogTestFixture: public testing::Test
{
  public:
    void SetUp()
    {
      /*Set up timer with 2 second timeout*/
      Clank::Configuration::Configuration *configuration = new Clank::Configuration::Configuration();

      configuration->AddConfigurationData("Timeout", "20");

      watchdog = new Clank::Timers::Watchdog(configuration);
      watchdog->AddTimeoutCallback(WatchdogCountFxn);
    }

    Clank::Timers::Watchdog *watchdog;
};

TEST_F(WatchdogTestFixture, WatchdogNotCalledWhenKicked)
{
  WatchdogExpireCount = 0;
  /*Kick the dog at 1 second intervals. This should not set off the watchdog*/
  watchdog->Start();
  for (int i = 0; i < 10; i++)
  {
    watchdog->Kick();
    usleep(10000);
  }
  watchdog->Stop();

  ASSERT_TRUE(WatchdogExpireCount == 0);
}

TEST_F(WatchdogTestFixture, WatchdogCalledWhenTimingOut)
{
  WatchdogExpireCount = 0;

  /*Wait for 5 seconds. This should cause the watchdog to be called at least twice*/
  watchdog->Start();
  for (int i = 0; i < 5; i++)
  {
    usleep(10000);
  }
  watchdog->Stop();

  ASSERT_TRUE(WatchdogExpireCount == 2);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
