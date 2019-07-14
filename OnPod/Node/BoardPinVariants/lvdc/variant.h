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
#define PINS_COUNT           (30u)
#define NUM_DIGITAL_PINS     (25u)
#define NUM_ANALOG_INPUTS    (5u)
#define NUM_ANALOG_OUTPUTS   (0u)
#define analogInputToDigitalPin(p)  ((p < 7u) ? (p) + 15u : -1)
// Low-level pin register query macros
#define digitalPinToPort(P)      (&(PORT->Group[g_APinDescription[P].ulPort]))
#define digitalPinToBitMask(P)   (1 << g_APinDescription[P].ulPin)
#define portOutputRegister(port) (&(port->OUT.reg))
#define portInputRegister(port)  (&(port->IN.reg))
#define portModeRegister(port)   (&(port->DIR.reg))
#define digitalPinHasPWM(P)      (g_APinDescription[P].ulPWMChannel != NOT_ON_PWM || g_APinDescription[P].ulTCChannel != NOT_ON_TIMER)


// Analog Pins AREF
// Internal SAMD stuff
#define PIN_A0               (0ul)
static const uint8_t A0   =  PIN_A0;
static const uint8_t DAC0 =  (0ul);
#define ADC_RESOLUTION		 12

// SPI Interfaces
#define SPI_INTERFACES_COUNT 2

// AIN PINS
#define PIN_AIN_0 1
#define PIN_AIN_1 2
#define PIN_AIN_2 3
#define PIN_AIN_3 4
static const uint8_t AIN_0 = PIN_AIN_0;
static const uint8_t AIN_1 = PIN_AIN_1;
static const uint8_t AIN_2 = PIN_AIN_2;
static const uint8_t AIN_3 = PIN_AIN_3;

// W5500 SPI
#define PIN_SPI_MISO  		10
#define PIN_SPI_MOSI  		9
#define PIN_SPI_SCK   		8
#define PIN_SPI_SS   		11
#define PERIPH_SPI   		sercom2
#define PAD_SPI_TX    		SPI_PAD_2_SCK_3
#define PAD_SPI_RX    		SERCOM_RX_PAD_1
static const uint8_t W5500_SS   = PIN_SPI_SS;
static const uint8_t W5500_MOSI = PIN_SPI_MOSI;
static const uint8_t W5500_MISO = PIN_SPI_MISO;
static const uint8_t W5500_SCK  = PIN_SPI_SCK;

// ADC SPI
#define PIN_SPI1_MISO 	 	 16
#define PIN_SPI1_MOSI 	 	 15
#define PIN_SPI1_SCK  	 	 14
#define PIN_SPI1_SS   	 	 17
#define PERIPH_SPI1   	     sercom0
#define PAD_SPI1_TX   	 	 SPI_PAD_3_SCK_1
#define PAD_SPI1_RX   		 SERCOM_RX_PAD_2
static const uint8_t ADC_SS   = PIN_SPI1_SS;
static const uint8_t ADC_MOSI = PIN_SPI1_MOSI;
static const uint8_t ADC_MISO = PIN_SPI1_MISO;
static const uint8_t ADC_SCK  = PIN_SPI1_SCK;

// CONVERTER ENABLE PINS
#define PIN_LP5V_EN     18
#define PIN_LP12V_EN    19
#define PIN_HP12V_EN    20
#define PIN_LP24V_EN    21
static const uint8_t LP5V_EN = PIN_LP5V_EN;
static const uint8_t LP12V_EN = PIN_LP12V_EN;
static const uint8_t HP12V_EN = PIN_HP12V_EN;
static const uint8_t LP24V_EN = PIN_LP24V_EN;

// HP TRANSISTORS
#define PIN_INV_CTL     22
#define PIN_PUMP1_CTL   23
#define PIN_PUMP2_CTL   24
static const uint8_t INV_CTL = PIN_INV_CTL;
static const uint8_t PUMP1_CTL = PIN_PUMP1_CTL;
static const uint8_t PUMP2_CTL = PIN_PUMP2_CTL;

// USB
#define PIN_USB_HOST_ENABLE 5
#define PIN_USB_DM          6
#define PIN_USB_DP          7

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
