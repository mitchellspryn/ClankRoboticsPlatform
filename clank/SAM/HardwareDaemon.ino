/*TODO: Re-write all of this using native AVR.h-style register manipulation*/

#include <string.h>

#include "libs/GpioPin.h"
#include "libs/AdcPin.h"
#include "libs/PwmPin.h"

#define BUF_SIZE 1024

#define UPDATE_MS 20
#define PACKET_TERMINATOR '\n'

/*Global Variables*/
char TxBuf[BUF_SIZE + 2]; //For elegance
char RxBuf[BUF_SIZE + 2];
char* TxBufPtr;
char* RxBufPtr;

unsigned long previousTime;

GpioPin GpioPins[32];
AdcPin AdcPins[12];
PwmPin PwmPins[12];

/*Function declarations*/
void ReportStatus();
void ProcessMessage();
void ResetAll();

void setup()
{
  Serial.begin(115200); //TODO: Faster baud?

  Serial.write("Beginning initialization process...\n");

  ResetAll();

  Serial.write("Done initializing!");
}

void loop()
{
  unsigned long currentTime = millis();
  if (currentTime - previousTime > UPDATE_MS)
  {
    previousTime = currentTime;
    ReportStatus();
  }

  while(Serial.available())
  {
    *RxBufPtr = Serial.read();
    if (*RxBufPtr == PACKET_TERMINATOR)
    {
      *RxBufPtr = '\0';
      ProcessMessage();
      RxBufPtr = RxBuf;
    }
    else
    {
      RxBufPtr++;
      if ((RxBufPtr - RxBuf) == BUF_SIZE)
      {
        RxBufPtr = RxBuf;
        Serial.write("Error: Buffer overflow");
      }
    }
  }
}

void ResetAll()
{
  int i;

  for (i = 0; i < BUF_SIZE + 1; i++)
  {
    TxBuf[i] = 0;
    RxBuf[i] = 0;
  }

  for (i = 0; i < 32; i++)
  {
    GpioPin_Init(&GpioPins[i], i+22);
  }
  for (i = 0; i < 12; i++)
  {
    AdcPin_Init(&AdcPins[i], i);
    PwmPin_Init(&PwmPins[i], i+2);
  }

  previousTime = millis();
  RxBufPtr = RxBuf;
  TxBufPtr = TxBuf;
}

void ReportStatus()
{
  int i;
  TxBufPtr = TxBuf;
  for (i = 0; i < BUF_SIZE + 1; i++)
  {
    TxBuf[i] = '\0';
  }
  for (i = 0; i < 32; i++)
  {
    int numCharWritten = GpioPin_Report(&GpioPins[i], TxBufPtr, BUF_SIZE - (TxBufPtr - TxBuf));
    if (numCharWritten < 0)
    {
      char error[64];
      sprintf(error, "Error report GPIO%d", GpioPins[i].Id);
      Serial.write(error);
      return;
    }
    TxBufPtr += numCharWritten;
  }
  for (i = 0; i < 12; i++)
  {
    int numCharWritten = AdcPin_Report(&AdcPins[i], TxBufPtr, BUF_SIZE - (TxBufPtr - TxBuf));
    if (numCharWritten < 0)
    {
      char error[64];
      sprintf(error, "Error report ADC%d", AdcPins[i].Id);
      Serial.write(error);
      return;
    }
    TxBufPtr += numCharWritten;
  }
  for (i = 0; i < 12; i++)
  {
    int numCharWritten = PwmPin_Report(&PwmPins[i], TxBufPtr, BUF_SIZE - (TxBufPtr - TxBuf));
    if (numCharWritten < 0)
    {
      char error[64];
      sprintf(error, "Error report PWM%d", PwmPins[i].Id);
      Serial.write(error);
      return;
    }
    TxBufPtr += numCharWritten;
  }
  
  if (TxBufPtr - TxBuf > 0)
  {
    char* workPtr = TxBuf;
    while(workPtr < TxBufPtr)
    {
      Serial.write(*workPtr);
      workPtr++;
    }
  }
}

/*TODO: Better error handling*/
void ProcessMessage()
{
  /*Ping test*/
  if (!strcmp(RxBuf, "Ping"))
  {
    Serial.println("Pong");
    return;
  }

  /*Reset message*/
  if (!strcmp(RxBuf, "Reset"))
  {
    ResetAll();
    Serial.println("Reset");
    return;
  }

  //Serial.print("Processing ");
  //Serial.println(RxBuf);
  char WorkBuf[BUF_SIZE + 1];
  strcpy(WorkBuf, RxBuf);
  char* firstToken = strtok(RxBuf, ":");
  if (!strncmp(firstToken, "GPIO", 4))
  {
    int id = (int)strtol(RxBuf+4, NULL, 10);
    if ( (id < 22) || (id > 53) )
    {
      Serial.write("Error: Invalid GPIO");
      return;
    }

    GpioPin_Process(&GpioPins[id-22], WorkBuf+7);
    return;
  }
  else if (!strncmp(firstToken, "ADC", 3))
  {
    int id = (int)strtol(firstToken+3, NULL, 10);
    if ( (id < 0) | (id > 11) )
    {
      Serial.write("Error: Invalid ADC");
      return;
    }

    AdcPin_Process(&AdcPins[id], WorkBuf+strlen(firstToken)+1);
  }
  else if (!strncmp(firstToken, "PWM", 3))
  {
    int id = (int)strtol(firstToken+3, NULL, 10);
    if ( (id < 2) || (id > 13) )
    {
      Serial.write("Error: Invalid PWM");
      return;
    }

    PwmPin_Process(&PwmPins[id-2], WorkBuf+strlen(firstToken)+1);
  }
  else
  {
    Serial.write("Error: Unsupported peripheral.");
  }

  int i;
  for (i = 0; i < BUF_SIZE + 1; i++)
  {
    RxBuf[i] = 0;
  }
  return;
}
