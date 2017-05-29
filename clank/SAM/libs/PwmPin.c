#include "PwmPin.h"

bool PwmPin_Set(PwmPin *pin);

bool PwmPin_Init(PwmPin *pin, int id)
{
  if ( (id < 2) || (id > 13) )
  {
    return false;
  }

  pin->Id = id;
  pin->Enabled = 0;
  pin->LastReportedEnabled = 0;
  pin->PwmValue = -1.0;
  pin->LastReportedPwmValue = -1.0;
  pin->SetPoint = 0.0;
  pin->RampSpeed = -1.0;
  pin->LastReportedRampSpeed = -1.0;
  PwmPin_Set(pin);
}

bool PwmPin_Enable(PwmPin *pin)
{
  pin->Enabled = 1;
  pin->PwmValue = 1;
  pin->SetPoint = 0;
  pin->RampSpeed = -1;
  PwmPin_Set(pin);
}

int PwmPin_Report(PwmPin *pin, char* buf, int spaceRemaining)
{
  char tmpBuf[64];
  int enabledChanged = 0;
  int msgLength = 0;

  if (pin->LastReportedEnabled != pin->Enabled)
  {
    pin->LastReportedEnabled = pin->Enabled;
    if (pin->Enabled == 1)
    {
      msgLength = sprintf(tmpBuf, "PWM%d:Enabled:\n", pin->Id);
    }
    else
    {
      msgLength = sprintf(tmpBuf, "PWM%d:Disabled:\n", pin->Id);
    }
    enabledChanged = 1;
  }

  else if (!pin->Enabled)
  {
    return 0;
  }

  PwmPin_Set(pin);

  if (!enabledChanged &&
    (
      (pin->PwmValue != pin->LastReportedPwmValue)
      ||
      (pin->RampSpeed != pin->LastReportedRampSpeed)
    )
  )
  {
    pin->LastReportedPwmValue = pin->PwmValue;
    pin->LastReportedRampSpeed = pin->RampSpeed;
    msgLength = sprintf(tmpBuf, "PWM%d:val:%.2f:set:%.2f:ramp:%.2f:\n", 
      pin->Id,
      pin->PwmValue,
      pin->SetPoint,
      pin->RampSpeed);
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

bool PwmPin_Process(PwmPin *pin, char* message)
{
  char* action = strtok(message, ":");
  if (!strncmp(action, "val", 3))
  {
    action = strtok(NULL, ":");
    
    /*Verify that the float value is actually a float*/
    if (!(strspn(action, "0123456789.") == strlen(action)))
    {
      return false;
    }

    float value = (float)strtod(action, NULL);

    action = strtok(NULL, ":");

    if (strncmp(action, "ramp", 4))
    {
      return false;
    }

    action = strtok(NULL, ":");
    if (!(strspn(action, "0123456789.-") == strlen(action)))
    {
      return false;
    }

    float rampSpeed = (float)strtod(action, NULL);
    pin->SetPoint = value;
    pin->RampSpeed = rampSpeed;
    return PwmPin_Set(pin);
  }

  /*Check for enable / disable*/
  else if (!strncmp(action, "enable", 6))
  {
    return PwmPin_Enable(pin);
  }
  else if (!strncmp(action, "disable", 8))
  {
    return PwmPin_Disable(pin);
  }
}

bool PwmPin_Set(PwmPin *pin)
{
  if (pin->SetPoint != pin->PwmValue && pin->SetPoint >= 0)
  {
    if (pin->RampSpeed <= 0)
    {
      pin->PwmValue = pin->SetPoint;
    }
    else
    {
      if (pin->SetPoint > pin->PwmValue)
      {
        {
          if (pin->SetPoint < pin->PwmValue + pin->RampSpeed)
          {
            pin->PwmValue = pin->SetPoint;
          }
          else
          {
            pin->PwmValue = pin->PwmValue + pin->RampSpeed;
          }
        }
      }

      else if (pin->SetPoint < pin->PwmValue)
      {
        if (pin->SetPoint > pin->PwmValue - pin->RampSpeed)
        {
          pin->PwmValue = pin->SetPoint;
        }
        else
        {
          pin->PwmValue = pin->PwmValue - pin->RampSpeed;
        }
      }
    }
    analogWrite(pin->Id, pin->PwmValue * 255.0 / 100.0);
  }
  
  return true;
}

bool PwmPin_Disable(PwmPin *pin)
{
  pinMode(pin->Id, INPUT);
  pin->Enabled = 0;
}
