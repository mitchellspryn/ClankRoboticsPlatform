#include "SamPeripheralExtentions.hpp"

using namespace std;
using namespace Clank::Hardware;

SamPeripheralName SamPeripheralExtentions::StringToPeripheralName(string input)
{
  if (!input.compare("DUMMYFIRST"))
  {
    throw runtime_error("SamPeripheralManager Error: Attempting to perform string to peripheral type conversion on invalid member DUMMYFIRST");
  }

  if (!input.compare("DUMMYLAST"))
  {
    throw runtime_error("SamPeripheralManager error: Attempting to perform string to peripheral type conversion on invalid member DUMMYLAST");
  }

  if (!input.compare("GPIO22"))
    return GPIO22;
  if (!input.compare("GPIO23"))
    return GPIO23;
  if (!input.compare("GPIO24"))
    return GPIO24;
  if (!input.compare("GPIO25"))
    return GPIO25;
  if (!input.compare("GPIO26"))
    return GPIO26;
  if (!input.compare("GPIO27"))
    return GPIO27;
  if (!input.compare("GPIO28"))
    return GPIO28;
  if (!input.compare("GPIO29"))
    return GPIO29;
  if (!input.compare("GPIO30"))
    return GPIO30;
  if (!input.compare("GPIO31"))
    return GPIO31;
  if (!input.compare("GPIO32"))
    return GPIO32;
  if (!input.compare("GPIO33"))
    return GPIO33;
  if (!input.compare("GPIO34"))
    return GPIO34;
  if (!input.compare("GPIO35"))
    return GPIO35;
  if (!input.compare("GPIO36"))
    return GPIO36;
  if (!input.compare("GPIO37"))
    return GPIO37;
  if (!input.compare("GPIO38"))
    return GPIO38;
  if (!input.compare("GPIO39"))
    return GPIO39;
  if (!input.compare("GPIO40"))
    return GPIO40;
  if (!input.compare("GPIO41"))
    return GPIO41;
  if (!input.compare("GPIO42"))
    return GPIO42;
  if (!input.compare("GPIO43"))
    return GPIO43;
  if (!input.compare("GPIO44"))
    return GPIO44;
  if (!input.compare("GPIO45"))
    return GPIO45;
  if (!input.compare("GPIO46"))
    return GPIO46;
  if (!input.compare("GPIO47"))
    return GPIO47;
  if (!input.compare("GPIO48"))
    return GPIO48;
  if (!input.compare("GPIO49"))
    return GPIO49;
  if (!input.compare("GPIO50"))
    return GPIO50;
  if (!input.compare("GPIO51"))
    return GPIO51;
  if (!input.compare("GPIO52"))
    return GPIO52;
  if (!input.compare("GPIO53"))
    return GPIO53;

  if (!input.compare("PWM2"))
    return PWM2;
  if (!input.compare("PWM3"))
    return PWM3;
  if (!input.compare("PWM4"))
    return PWM4;
  if (!input.compare("PWM5"))
    return PWM5;
  if (!input.compare("PWM6"))
    return PWM6;
  if (!input.compare("PWM7"))
    return PWM7;
  if (!input.compare("PWM8"))
    return PWM8;
  if (!input.compare("PWM9"))
    return PWM9;
  if (!input.compare("PWM10"))
    return PWM10;
  if (!input.compare("PWM11"))
    return PWM11;
  if (!input.compare("PWM12"))
    return PWM12;
  if (!input.compare("PWM13"))
    return PWM13;

  if (!input.compare("ADC0"))
    return ADC0;
  if (!input.compare("ADC1"))
    return ADC1;
  if (!input.compare("ADC2"))
    return ADC2;
  if (!input.compare("ADC3"))
    return ADC3;
  if (!input.compare("ADC4"))
    return ADC4;
  if (!input.compare("ADC5"))
    return ADC5;
  if (!input.compare("ADC6"))
    return ADC6;
  if (!input.compare("ADC7"))
    return ADC7;
  if (!input.compare("ADC8"))
    return ADC8;
  if (!input.compare("ADC9"))
    return ADC9;
  if (!input.compare("ADC10"))
    return ADC10;
  if (!input.compare("ADC11"))
    return ADC11;

  if (!input.compare("DAC0"))
    return DAC0;
  if (!input.compare("DAC1"))
    return DAC1;

  if (!input.compare("TWI0"))
    return TWI0;
  if (!input.compare("TWI1"))
    return TWI1;

  if (!input.compare("SPI0"))
    return SPI0;

  if (!input.compare("USART0"))
    return USART0;
  if (!input.compare("USART1"))
    return USART1;
  if (!input.compare("USART2"))
    return USART2;
  if (!input.compare("USART3"))
    return USART3;
  if (!input.compare("CAN0")) 
    return CAN0;

  throw runtime_error("SamPeripheralManager: Unrecognized peripheral in StringToPeripheralType()");
}

string SamPeripheralExtentions::PeripheralNameToString(SamPeripheralName p)
{
  /*Never operate on dummy objects!*/
  switch (p)
  {
    case DUMMYFIRST:
      throw runtime_error("SamPeripheralManger Error: Attempting to perform peripheral to string type conversion on invalid peripheral DUMMYFIRST");
    case DUMMYLAST:
      throw runtime_error("SamPeripheralManager Error: Attempting to perform peripheal to string type conversion on invalid peripheral DUMMYLAST");

    case GPIO22:
      return "GPIO22";
    case GPIO23:
      return "GPIO23";
    case GPIO24:
      return "GPIO24";
    case GPIO25:
      return "GPIO25";
    case GPIO26:
      return "GPIO26";
    case GPIO27:
      return "GPIO27";
    case GPIO28:
      return "GPIO28";
    case GPIO29:
      return "GPIO29";
    case GPIO30:
      return "GPIO30";
    case GPIO31:
      return "GPIO31";
    case GPIO32:
      return "GPIO32";
    case GPIO33:
      return "GPIO33";
    case GPIO34:
      return "GPIO34";
    case GPIO35:
      return "GPIO35";
    case GPIO36:
      return "GPIO36";
    case GPIO37:
      return "GPIO37";
    case GPIO38:
      return "GPIO38";
    case GPIO39:
      return "GPIO39";
    case GPIO40:
      return "GPIO40";
    case GPIO41:
      return "GPIO41";
    case GPIO42:
      return "GPIO42";
    case GPIO43:
      return "GPIO43";
    case GPIO44:
      return "GPIO44";
    case GPIO45:
      return "GPIO45";
    case GPIO46:
      return "GPIO46";
    case GPIO47:
      return "GPIO47";
    case GPIO48:
      return "GPIO48";
    case GPIO49:
      return "GPIO49";
    case GPIO50:
      return "GPIO50";
    case GPIO51:
      return "GPIO51";
    case GPIO52:
      return "GPIO52";
    case GPIO53:
      return "GPIO53";

    case  PWM2:
      return "PWM2";
    case  PWM3:
      return "PWM3";
    case  PWM4:
      return "PWM4";
    case  PWM5:
      return "PWM5";
    case  PWM6:
      return "PWM6";
    case  PWM7:
      return "PWM7";
    case  PWM8:
      return "PWM8";
    case  PWM9:
      return "PWM9";
    case PWM10:
      return "PWM10";
    case PWM11:
      return "PWM11";
    case PWM12:
      return "PWM12";
    case PWM13:
      return "PWM13";

    case ADC0:
      return "ADC0";
    case ADC1:
      return "ADC1";
    case ADC2:
      return "ADC2";
    case ADC3:
      return "ADC3";
    case ADC4:
      return "ADC4";
    case ADC5:
      return "ADC5";
    case ADC6:
      return "ADC6";
    case ADC7:
      return "ADC7";
    case ADC8:
      return "ADC8";
    case ADC9:
      return "ADC9";
    case ADC10:
      return "ADC10";
    case ADC11:
      return "ADC11";

    case DAC0:
      return "DAC0";
    case DAC1:
      return "DAC1";

    case TWI0:
      return "TWI0";
    case TWI1:
      return "TWI1";

    case SPI0:
      return "SPI0";

    case USART0:
      return "USART0";
    case USART1:
      return "USART1";
    case USART2:
      return "USART2";
    case USART3:
      return "USART3";
    case CAN0: 
      return "CAN0";

    default:
      throw runtime_error("SamPeripheralManager: Unrecognized peripheral in PeripheralTypeToString()");
  }
}
