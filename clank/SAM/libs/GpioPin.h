#ifndef GPIOPIN_H
#define GPIOPIN_H

#include "Arduino.h"
#include <string.h>

typedef struct GpioPin_t
{
  /*ENABLED / DISABLED*/
  int Enabled;
  int LastReportedEnabled;

  /*INPUT, OUTPUT*/
  int Direction;
  int LastReportedDirection;

  /*HIGH, LOW*/
  int Value;
  int LastReportedValue;

  /*The Arduino pin the GPIO corresponds to*/
  /*Corresponds to GPIOxx in SamPeripheralExtentions.hpp*/
  int Id;
} GpioPin;

bool GpioPin_Init(GpioPin *pin, int Id);
bool GpioPin_Enable(GpioPin *pin);
int  GpioPin_Report(GpioPin *pin, char* buf, int spaceRemaining);
bool GpioPin_Process(GpioPin *pin, char* message);
bool GpioPin_Disable(GpioPin *pin);

#endif
