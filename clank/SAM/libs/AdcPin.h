#ifndef ADCPIN_H
#define ADCPIN_H

#include "Arduino.h"
#include <string.h>
#include <math.h>

typedef struct AdcPin_t
{
  /*ENABLED, DISABLED*/
  int Enabled;
  int LastReportedEnabled;

  float Threshold;
  float LastReportedThreshold;

  float LastReportedValue;

  int Id;
} AdcPin;


bool AdcPin_Init(AdcPin *pin, int Id);
bool AdcPin_Enable(AdcPin *pin);
int AdcPin_Report(AdcPin *pin, char* buf, int spaceRemaining);
bool AdcPin_Process(AdcPin *pin, char* message);
bool AdcPin_Disable(AdcPin *pin);

#endif

