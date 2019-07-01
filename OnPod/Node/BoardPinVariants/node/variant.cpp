/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "variant.h"
#include <WVariant.h>

/********** The order here matters, always add new pinouts to the bottom **********/
/* The number next to the tag is the pin number to use in code, it is the pins    */
/* index in the g_APinDescription[], enusre this matches pincount in variant.h    */
const PinDescription g_APinDescription[] = {
////// ANALOG REFERENCE //////
// 0 A0
  { PORTA, 2, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel0, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2 },

////// External Sercom (ICSP:MISO,SCK,MOSI)//////
// 1 MISO/GPIO wedit
  { PORTA, 21, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 2 SCLK/UART RX
  { PORTA, 23, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 3 MOSI/UART TX
  { PORTA, 22, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },


////// USB //////
// 4 USB Host Enable
  { PORTA, 28, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 5 USB -
  { PORTA, 24, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DM
// 6 USB +
  { PORTA, 25, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DP


////// W5500 SPI //////
// 7 PIN_SPI_SS
  { PORTA,  4, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 8 PIN_SPI_SCK
  { PORTA,  7, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_6 },
// 9 PIN_SPI_MOSI
  { PORTA,  6, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_7 },
// 10 PIN_SPI_MISO
  { PORTA,  5, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_5 },

////// W5500 GPIO //////
// 11 NETW_INT_L
  { PORTB,  8, PIO_DIGITAL, PIN_ATTR_DIGITAL, ADC_Channel2, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_8 },
// 12 NETW_RESET_L
  { PORTB,  9, PIO_DIGITAL, PIN_ATTR_DIGITAL, ADC_Channel2, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_9 },


////// ADC SPI1 //////
// 13 PIN_SPI1_SS
  { PORTA, 9, PIO_DIGITAL, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 14 PIN_SPI1_SCK
  { PORTA, 11, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 15 PIN_SPI1_MOSI
  { PORTA, 8, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 16 PIN_SPI1_MISO
  { PORTA, 10, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },

////// ADC POWER //////
// 17 POWER_SEQ_ADC
  { PORTA, 3, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },


////// DAC SPI2 //////
// 18 PIN_SPI2_SS
  { PORTA, 16, PIO_DIGITAL, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 19 PIN_SPI2_SCK
  { PORTA, 13, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 20 PIN_SPI2_MOSI
  { PORTB, 11, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 21 PIN_SPI2_MISO
  { PORTB, 10, PIO_SERCOM_ALT, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },

////// DAC GPIO //////
// 22 DAC_CLEAR_I
  { PORTA,  19, PIO_DIGITAL, PIN_ATTR_DIGITAL, ADC_Channel2, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 23 DAC_ALARM_L
  { PORTA,  20, PIO_EXTINT, PIN_ATTR_DIGITAL, ADC_Channel2, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_4 },

////// DAC POWER //////
// 24 POWER_SEQ_DAC
  { PORTB,  2, PIO_DIGITAL, PIN_ATTR_DIGITAL, ADC_Channel2, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 25 DAC_SEQ_START
  { PORTB,  3, PIO_DIGITAL, PIN_ATTR_DIGITAL, ADC_Channel2, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },

////// SOLENOID DRIVERS SERIAL //////
// 26 SOL_CS PA12
{ PORTA, 12, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 27 DATA_IN PB22
{ PORTB, 22, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 28 CLK PA27
{ PORTA, 27, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 29 DATAOUT PB23
{ PORTB, 23, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },

////// SOLENOID DRIVERS GPIO ///////
// 30 SOL_FAULT_LOW PA17
{ PORTA, 17, PIO_EXTINT, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_1 },
// 31 SOL_ENABLE PA18
{ PORTA, 18, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },

/*
////// SWD //////
// 13 PA30 SWD_CLK placeholder
  { NOT_A_PORT, 0, PIO_NOT_A_PIN, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 14 PA31 SWDIO placeholder
  { NOT_A_PORT, 0, PIO_NOT_A_PIN, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },
// 15 RESET_L placeholder
  { NOT_A_PORT, 0, PIO_NOT_A_PIN, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE },


// power sqeuencer pin 6 AREF

*/
};

// Multi-serial objects instantiation
SERCOM sercom0(SERCOM0);
SERCOM sercom1(SERCOM1);
SERCOM sercom2(SERCOM2);
SERCOM sercom3(SERCOM3);
SERCOM sercom4(SERCOM4);
SERCOM sercom5(SERCOM5);

// ExtSerial
Uart ExtSerial(&sercom5, 2, 3, SERCOM_RX_PAD_1, UART_TX_PAD_0);
void SERCOM5_Handler()
{
	ExtSerial.IrqHandler();
}
