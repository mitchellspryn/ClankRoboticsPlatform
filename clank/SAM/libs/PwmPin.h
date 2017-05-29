#ifndef PWMPIN_H
#define PWMPIN_H

#include "Arduino.h"
#include <string.h>

typedef struct PwmPin_t
{
  int Enabled;
  int LastReportedEnabled;

  /*Between 0.0 and 100.0*/
  float PwmValue; 
  float LastReportedPwmValue;

  float SetPoint;

  float RampSpeed;
  float LastReportedRampSpeed;

  int Id;
} PwmPin;

bool PwmPin_Init(PwmPin *pin, int id);
bool PwmPin_Enable(PwmPin *pin);
int  PwmPin_Report(PwmPin *pin, char* buf, int spaceRemaining);
bool PwmPin_Process(PwmPin *pin, char* message);
bool PwmPin_Disable(PwmPin *pin);

#endif
