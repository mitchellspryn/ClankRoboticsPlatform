#include "AdcPin.h"

bool AdcPin_Init(AdcPin *pin, int id)
{
  if ( (id < 0) || (id > 11) )
  {
    return false;
  }

  pin->Id = id;
  pin->Enabled = 0;
  pin->LastReportedEnabled = 0;
  pin->Threshold = 5.0;
  pin->LastReportedThreshold = 5.0;
  pin->LastReportedValue = 0;
  return true;
}

bool AdcPin_Enable(AdcPin *pin)
{
  pin->Enabled = 1;
  return true;
}

int AdcPin_Report(AdcPin *pin, char* buf, int spaceRemaining)
{
  char tmpBuf[64];
  int enabledChanged = 0;
  int msgLength = 0;

  if (pin->LastReportedEnabled != pin->Enabled)
  {
    pin->LastReportedEnabled = pin->Enabled;
    if (pin->Enabled == 1)
    {
      msgLength = sprintf(tmpBuf, "ADC%d:Enabled:\n", pin->Id);
    }
    else
    {
      msgLength = sprintf(tmpBuf, "ADC%d:Disabled:\n", pin->Id);
    }
    enabledChanged = 1;
  }

  else if (!pin->Enabled)
  {
    return 0;
  }

  float value = (float)(((float)analogRead(pin->Id) * (float)100.0) / (float)1024.0);

  if ( !enabledChanged && 
    (
      (pin->Threshold != pin->LastReportedThreshold)
      ||
      (abs(value - pin->LastReportedValue) > pin->Threshold)
    )
  )
  {
    pin->LastReportedThreshold = pin->Threshold;
    pin->LastReportedValue = value;
    msgLength = sprintf(tmpBuf, "ADC%d:thresh:%.2f:val:%.2f:\n", 
      pin->Id, 
      pin->LastReportedThreshold,
      value);
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

bool AdcPin_Process(AdcPin *pin, char* message)
{
  char* action = strtok(NULL, ":");

  /*Check for enable / disable*/
  if (!strncmp(action, "enable", 6))
  {
    return AdcPin_Enable(pin);
  }
  else if (!strncmp(action, "disable", 8))
  {
    return AdcPin_Disable(pin);
  }
  else if (!strncmp(action, "thresh", 6))
  {
    action = strtok(NULL, ":");
    float threshold = (float)strtod(action, NULL);
    if (threshold <= 0)
    {
      return false;
    }
    pin->Threshold = threshold;
    return true;
  }

  return false;
}

bool AdcPin_Disable(AdcPin *pin)
{
  pin->Enabled = 0;
  return true;
}
