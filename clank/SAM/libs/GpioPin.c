#include "GpioPin.h"

bool GpioPin_Set(GpioPin *pin, int direction , int value);
bool GpioPin_Read(GpioPin *pin);

bool GpioPin_Init(GpioPin *pin, int id)
{
  if ((id < 22) || (id > 53))
  {
    return false; 
  }
  pin->Id = id;
  pin->Enabled = 0;
  pin->LastReportedEnabled = 0;
  pin->LastReportedDirection = -1;
  pin->LastReportedValue = -1;
  GpioPin_Set(pin, INPUT, HIGH);
}

bool GpioPin_Enable(GpioPin *pin)
{
  pin->Enabled = 1;
  return true;
}

int GpioPin_Report(GpioPin *pin, char* buf, int spaceRemaining)
{
  char tmpBuf[64];
  int enabledChanged = 0;
  int msgLength = 0;
  if (pin->LastReportedEnabled != pin->Enabled)
  {
    pin->LastReportedEnabled = pin->Enabled;
    if (pin->Enabled == 1)
    {
      msgLength = sprintf(tmpBuf, "GPIO%d:Enabled:\n", pin->Id);
    }
    else
    {
      msgLength = sprintf(tmpBuf, "GPIO%d:Disabled:\n", pin->Id);
    }
    enabledChanged = 1;
  }

  else if (!pin->Enabled)
  {
    return 0;
  }

  /*Read from pin if necessary*/
  if (pin->Direction == INPUT || pin->Direction == INPUT_PULLUP)
  {
    GpioPin_Read(pin);
  }

  if (!enabledChanged && (pin->LastReportedDirection != pin->Direction || pin->LastReportedValue != pin->Value))
  {

    pin->LastReportedDirection = pin->Direction;
    pin->LastReportedValue = pin->Value;

    msgLength = sprintf(tmpBuf, "GPIO%d:dir:%s:val:%s:\n", pin->Id,
      (pin->Direction == OUTPUT ? "Out" : "In"),
      (pin->Value == HIGH ? "Hi" : "Low"));
  }

  if (msgLength > spaceRemaining)
  {
    return -1;
  }

  else if (msgLength > 0)
  {
    strcpy(buf, tmpBuf);
  }

  return msgLength;
}

bool GpioPin_Process(GpioPin *pin, char* message)
{
  char* action = strtok(message, ":");

  /*Check for setting parameters*/
  if (!strncmp(action, "dir", 3))
  {
    action = strtok(NULL, ":");
    int direction;
    if (!strncmp(action, "In", 2))
    {
      direction = INPUT;
    }
    else if (!strncmp(action, "Out", 3))
    {
      direction = OUTPUT;
    }
    else 
    {
      return false;
    }

    action = strtok(NULL, ":");
    if (strncmp(action, "val", 3))
    {
      return false;
    }

    action = strtok(NULL, ":");
    int value;
    if (!strncmp(action, "Hi", 2))
    {
      value = HIGH;
    }
    else if (!strncmp(action, "Low", 3))
    {
      value = LOW;
    }
    else
    {
      return false;
    }

    return GpioPin_Set(pin, direction, value);
  }

  /*Check for enable / disable*/
  else if (!strncmp(action, "enable", 6))
  {
    return GpioPin_Enable(pin);
  }
  else if (!strncmp(action, "disable", 8))
  {
    return GpioPin_Disable(pin);
  }
  
  return false;
}

bool GpioPin_Set(GpioPin *pin, int direction, int value)
{
  if (direction == OUTPUT)
  {
    pinMode(pin->Id, OUTPUT);
    if (value == HIGH)
    {
      digitalWrite(pin->Id, HIGH);
    }
    else
    {
      digitalWrite(pin->Id, LOW);
    }
  }
  else
  {
    if (value == LOW)
    {
      pinMode(pin->Id, INPUT);
    }
    else
    {
      pinMode(pin->Id, INPUT_PULLUP);
    }
  }

  pin->Direction = direction;
  pin->Value = value;
  return true; 
}

bool GpioPin_Read(GpioPin *pin)
{
  if (pin->Direction == INPUT || pin->Direction == INPUT_PULLUP)
  {
    pin->Value = digitalRead(pin->Id);
  }
  return true;
}

bool GpioPin_Disable(GpioPin *pin)
{
  GpioPin_Set(pin, INPUT, LOW);
  pin->Enabled = 0;
  return true;
}
