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

#pragma once

// The definitions here needs a SAMD core >=1.6.10
#define ARDUINO_SAMD_VARIANT_COMPLIANCE 10610
#include <WVariant.h>

// Frequency of the board main oscillator
#define VARIANT_MAINOSC (32768ul)

// Master clock frequency
#define VARIANT_MCK     (48000000ul)

// Number of pins defined in PinDescription array
#define PINS_COUNT           (34u)
#define NUM_DIGITAL_PINS     (8u)
#define NUM_ANALOG_INPUTS    (1u)
#define NUM_ANALOG_OUTPUTS   (0u)
#define analogInputToDigitalPin(p)  ((p < 7u) ? (p) + 15u : -1)
// Low-level pin register query macros
#define digitalPinToPort(P)      (&(PORT->Group[g_APinDescription[P].ulPort]))
#define digitalPinToBitMask(P)   (1 << g_APinDescription[P].ulPin)
#define portOutputRegister(port) (&(port->OUT.reg))
#define portInputRegister(port)  (&(port->IN.reg))
#define portModeRegister(port)   (&(port->DIR.reg))
#define digitalPinHasPWM(P)      (g_APinDescription[P].ulPWMChannel != NOT_ON_PWM || g_APinDescription[P].ulTCChannel != NOT_ON_TIMER)

// Node pins
#define NW_CS 				 7u

// Anaog Pins AREF
#define PIN_A0               (6ul)
static const uint8_t A0   = PIN_A0;
static const uint8_t DAC0 =  (6ul);
#define ADC_RESOLUTION		  12

// SPI Interfaces
#define SPI_INTERFACES_COUNT 1

// SPI - Wiznet
#define PIN_SPI_MISO  		8
#define PIN_SPI_MOSI  		10
#define PIN_SPI_SCK   		9
#define PIN_SPI_SS   		7
#define PERIPH_SPI   		sercom0
#define PAD_SPI_TX    		SPI_PAD_2_SCK_3
#define PAD_SPI_RX    		SERCOM_RX_PAD_1
static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

// SPI1 - ADC's
#define PIN_SPI1_MISO 	 	 17
#define PIN_SPI1_MOSI 	 	 19
#define PIN_SPI1_SCK  	 	 18
#define PIN_SPI1_SS   	 	 16
#define PERIPH_SPI1   	 	 sercom2
#define PAD_SPI1_TX   	 	 SPI_PAD_2_SCK_3
#define PAD_SPI1_RX   		 SERCOM_RX_PAD_0
static const uint8_t SS1   = PIN_SPI1_SS;
static const uint8_t MOSI1 = PIN_SPI1_MOSI;
static const uint8_t MISO1 = PIN_SPI1_MISO;
static const uint8_t SCK1  = PIN_SPI1_SCK;

// USB
#define PIN_USB_HOST_ENABLE (3ul)
#define PIN_USB_DM          (4ul)
#define PIN_USB_DP          (5ul)

// Serial ports
#ifdef __cplusplus
#include "SERCOM.h"
#include "Uart.h"

// Instances of SERCOM
extern SERCOM sercom0;
extern SERCOM sercom1;
extern SERCOM sercom2;
extern SERCOM sercom3;
extern SERCOM sercom4;
extern SERCOM sercom5;

extern Uart Serial; 	// Serial Monitor
extern Uart ExtSerial;  // Serial on External Sercom

#endif // __cplusplus

// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
#define SERIAL_PORT_MONITOR         SerialUSB
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
#define SERIAL_PORT_USBVIRTUAL      SerialUSB
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
#define SERIAL_PORT_HARDWARE        ExtSerial
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.
#define SERIAL_PORT_HARDWARE_OPEN   ExtSerial
// Alias Serial to SerialUSB
#define Serial                      SerialUSB

////////// Extra Examples /////////////

// Serial1
/*#define PIN_SERIAL1_RX       (0ul)
#define PIN_SERIAL1_TX       (1ul)
#define PAD_SERIAL1_TX       (UART_TX_PAD_2)
#define PAD_SERIAL1_RX       (SERCOM_RX_PAD_3)
*/

// Wire Interfaces (I2C)
/*#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA        (11u)
#define PIN_WIRE_SCL        (12u)
#define PERIPH_WIRE         sercom0
#define WIRE_IT_HANDLER     SERCOM0_Handler
static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;
*/

// I2S Interfaces
/*#define I2S_INTERFACES_COUNT 1

#define I2S_DEVICE          0
#define I2S_CLOCK_GENERATOR 3
#define PIN_I2S_SD          (PIN_A6)
#define PIN_I2S_SCK         (2u)
#define PIN_I2S_FS          (3u)
*/
