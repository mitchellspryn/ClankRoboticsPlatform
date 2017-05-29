#ifndef SAMPERIPHERALEXTENTIONS_HPP
#define SAMPERIPHERALEXTENTIONS_HPP

#include <string>
#include <stdexcept>

namespace Clank
{
  namespace Hardware
  {
    /*SAM Peripherals grabbed from here: http://www.open-electronics.org/wp-content/uploads/2014/04/udoo_Pinout.png*/
    enum SamPeripheralName
    {
      /*Dummy header for iteration*/
      DUMMYFIRST = 0,

      /*ADC Pins (Ax)*/
      ADC0,
      ADC1,
      ADC2,
      ADC3,
      ADC4,
      ADC5,
      ADC6,
      ADC7,
      ADC8,
      ADC9,
      ADC10,
      ADC11,

      /*DACs (DACx) */
      DAC0,
      DAC1,

      /*GPIO Pins (Dx) */

      GPIO22,
      GPIO23,
      GPIO24,
      GPIO25,
      GPIO26,
      GPIO27,
      GPIO28,
      GPIO29,
      GPIO30,
      GPIO31,
      GPIO32,
      GPIO33,
      GPIO34,
      GPIO35,
      GPIO36,
      GPIO37,
      GPIO38,
      GPIO39,
      GPIO40,
      GPIO41,
      GPIO42,
      GPIO43,
      GPIO44,
      GPIO45,
      GPIO46,
      GPIO47,
      GPIO48,
      GPIO49,
      GPIO50,
      GPIO51,
      GPIO52,
      GPIO53,

      /*CAN bus (CANRx, CANTx)*/
      CAN0,

      /*USART (Tx0, Tx1)*/
      USART0,
      USART1,
      USART2,
      USART3,

      /*SPI* (MOSI, MISO, SCK)*/
      SPI0,

      /*TWI buses (SDAx, SCLx)*/
      TWI0,
      TWI1,

      /*PWM pins (PWMx)*/
      PWM2,
      PWM3,
      PWM4,
      PWM5,
      PWM6,
      PWM7,
      PWM8,
      PWM9,
      PWM10,
      PWM11,
      PWM12,
      PWM13,

      /*Dummy footer for iteration*/
      DUMMYLAST
    };

    /*Helper functions*/
    class SamPeripheralExtentions
    {
      public:
        static std::string PeripheralNameToString(Clank::Hardware::SamPeripheralName p);
        static Clank::Hardware::SamPeripheralName StringToPeripheralName(std::string s);
    };
  }
}
#endif
