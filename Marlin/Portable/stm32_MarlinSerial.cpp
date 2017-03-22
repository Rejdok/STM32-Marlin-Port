/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 MarlinSerial.cpp - Hardware serial library for Wiring
 Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

 Modified 23 November 2006 by David A. Mellis
 Modified 28 September 2010 by Mark Sproul
 Modified 14 February 2016 by Andreas Hardtung (added tx buffer)


 */

#include "stm32_MarlinSerial.h"
#include "stm32_stepper.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"
#include "Marlin.h"

#ifdef __cplusplus
extern "C" {
void store_char(unsigned char c);
}
#endif
// this next line disables the entire HardwareSerial.cpp,
// this is so I can support Attiny series and any other chip without a UART

ring_buffer_r rx_buffer = { { 0 }, 0, 0 };
ring_buffer_t tx_buffer = { { 0 }, 0, 0 };

void store_char(unsigned char c) {
	CRITICAL_SECTION_START;
	uint8_t h = rx_buffer.head;
	uint8_t i = (uint8_t) ( h + 1 ) & ( RX_BUFFER_SIZE - 1 );
//     if we should be storing the received character into the location
//     just before the tail (meaning that the head would advance to the
//     current location of the tail), we're about to overflow the buffer
//     and so we don't write the character or advance the head.
	if ( i != rx_buffer.tail ) {
		rx_buffer.buffer[h] = c;
		rx_buffer.head = i;
	} CRITICAL_SECTION_END;

#if ENABLED(EMERGENCY_PARSER)
	emergency_parser(c);
#endif
}

#if TX_BUFFER_SIZE > 0

// Constructors ////////////////////////////////////////////////////////////////

MarlinSerial::MarlinSerial() {
}

// Public Methods //////////////////////////////////////////////////////////////

void MarlinSerial::begin(long baud) {

}

void MarlinSerial::end() {

}

void MarlinSerial::checkRx(void) {

}

int MarlinSerial::peek(void) {
	int v = 0;
	CRITICAL_SECTION_START;
	uint8_t t = rx_buffer.tail;
	if ( rx_buffer.head == t ) {
		v = -1;
	}
	else {
		v = rx_buffer.buffer[t];
	} CRITICAL_SECTION_END;
	return v;
}

int MarlinSerial::read(void) {
	int v = 0;
	CRITICAL_SECTION_START;
	uint8_t t = rx_buffer.tail;
	if ( rx_buffer.head == t ) {
		v = -1;
	}
	else {
		v = rx_buffer.buffer[t];
		rx_buffer.tail = (uint8_t) ( t + 1 ) & ( RX_BUFFER_SIZE - 1 );
	} CRITICAL_SECTION_END;
	return v;
}

uint8_t MarlinSerial::available(void) {
	CRITICAL_SECTION_START;
	uint8_t h = rx_buffer.head;
	uint8_t t = rx_buffer.tail;
	CRITICAL_SECTION_END;
	return (uint8_t) ( RX_BUFFER_SIZE + h - t ) & ( RX_BUFFER_SIZE - 1 );
}

void MarlinSerial::flush(void) {
	// RX
	// don't reverse this or there may be problems if the RX interrupt
	// occurs after reading the value of rx_buffer_head but before writing
	// the value to rx_buffer_tail; the previous value of rx_buffer_head
	// may be written to rx_buffer_tail, making it appear as if the buffer
	// were full, not empty.
	CRITICAL_SECTION_START;
	rx_buffer.head = rx_buffer.tail;
	CRITICAL_SECTION_END;
}

#if TX_BUFFER_SIZE > 0
uint8_t MarlinSerial::availableForWrite(void) {
	CRITICAL_SECTION_START;
	uint8_t h = tx_buffer.head;
	uint8_t t = tx_buffer.tail;
	CRITICAL_SECTION_END;
	return (uint8_t) ( TX_BUFFER_SIZE + h - t ) & ( TX_BUFFER_SIZE - 1 );
}

//  void MarlinSerial::write(const char c) {
//	if(hUsbDeviceFS.dev_state==3)
//	  CDC_Transmit_FS(c,1);
//  }

void MarlinSerial::flushTX(void) {
	// TX
	// If we have never written a byte, no need to flush. This special
	// case is needed since there is no way to force the TXC (transmit
	// complete) bit to 1 during initialization
	/*todo разобраться с парсером*/
//    if (!_written)
//      return;
//
//    while (TEST(M_UCSRxB, M_UDRIEx) || !TEST(M_UCSRxA, M_TXCx)) {
//      if (!TEST(SREG, SREG_I) && TEST(M_UCSRxB, M_UDRIEx))
//        // Interrupts are globally disabled, but the DR empty
//        // interrupt should be enabled, so poll the DR empty flag to
//        // prevent deadlock
//        if (TEST(M_UCSRxA, M_UDREx))
//          _tx_udr_empty_irq();
	//  }
	// If we get here, nothing is queued anymore (DRIE is disabled) and
	// the hardware finished tranmission (TXC is set).
}

void MarlinSerial::write(char c) {
	/*todo если usb оборветься девайс может повиснуть, надо переделать*/
	uint8_t rec = c;
	while ( 1 )
		if ( hUsbDeviceFS.dev_state == 3 ) {
			if ( !CDC_Transmit_FS( ( &rec ), 1) ) //CDC returns 1 when USB busy
				break;
			HAL_Delay(1);
		}
}

#endif
// end NEW

/// imports from print.h

void MarlinSerial::print(char c, int base) {
	print((long) c, base);
}

void MarlinSerial::print(unsigned char b, int base) {
	print((unsigned long) b, base);
}

void MarlinSerial::print(int n, int base) {
	print((long) n, base);
}

void MarlinSerial::print(unsigned int n, int base) {
	print((unsigned long) n, base);
}

void MarlinSerial::print(long n, int base) {
	if ( base == 0 ) {
		write(n);
	}
	else if ( base == 10 ) {
		if ( n < 0 ) {
			print('-');
			n = -n;
		}
		printNumber(n, 10);
	}
	else {
		printNumber(n, base);
	}
}

void MarlinSerial::print(unsigned long n, int base) {
	if ( base == 0 )
		write(n);
	else
		printNumber(n, base);
}

void MarlinSerial::print(double n, int digits) {
	printFloat(n, digits);
}

void MarlinSerial::println(void) {
	print('\r');
	print('\n');
}

//void MarlinSerial::println(const String& s) {
//  print(s);
//  println();
//}

void MarlinSerial::println(const char c[]) {
	print(c);
	println();
}

void MarlinSerial::println(char c, int base) {
	print(c, base);
	println();
}

void MarlinSerial::println(unsigned char b, int base) {
	print(b, base);
	println();
}

void MarlinSerial::println(int n, int base) {
	print(n, base);
	println();
}

void MarlinSerial::println(unsigned int n, int base) {
	print(n, base);
	println();
}

void MarlinSerial::println(long n, int base) {
	print(n, base);
	println();
}

void MarlinSerial::println(unsigned long n, int base) {
	print(n, base);
	println();
}

void MarlinSerial::println(double n, int digits) {
	print(n, digits);
	println();
}

// Private Methods /////////////////////////////////////////////////////////////

void MarlinSerial::printNumber(unsigned long n, uint8_t base) {
	unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars.
	unsigned long i = 0;

	if ( n == 0 ) {
		print('0');
		return;
	}

	while ( n > 0 ) {
		buf[i++] = n % base;
		n /= base;
	}

	for ( ; i > 0; i-- )
		print(
				(char) (
						buf[i - 1] < 10 ?
								'0' + buf[i - 1] : 'A' + buf[i - 1] - 10 ));
}

void MarlinSerial::printFloat(double number, uint8_t digits) {
	// Handle negative numbers
	if ( number < 0.0 ) {
		print('-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for ( uint8_t i = 0; i < digits; ++i )
		rounding /= 10.0;

	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long int_part = (unsigned long) number;
	double remainder = number - (double) int_part;
	print(int_part);

	// Print the decimal point, but only if there are digits beyond
	if ( digits > 0 )
		print('.');

	// Extract digits from the remainder one at a time
	while ( digits-- > 0 ) {
		remainder *= 10.0;
		int toPrint = int(remainder);
		print(toPrint);
		remainder -= toPrint;
	}
}
// Preinstantiate Objects //////////////////////////////////////////////////////

MarlinSerial customizedSerial;

#endif // whole file

// For AT90USB targets use the UART for BT interfacing
#if defined(USBCON) && ENABLED(BLUETOOTH)
HardwareSerial bluetoothSerial;
#endif

#if ENABLED(EMERGENCY_PARSER)

// Currently looking for: M108, M112, M410
// If you alter the parser please don't forget to update the capabilities in Conditionals_post.h

FORCE_INLINE void emergency_parser(unsigned char c) {

	static e_parser_state state = state_RESET;

	switch (state) {
		case state_RESET:
		switch (c) {
			case ' ': break;
			case 'N': state = state_N; break;
			case 'M': state = state_M; break;
			default: state = state_IGNORE;
		}
		break;

		case state_N:
		switch (c) {
			case '0': case '1': case '2':
			case '3': case '4': case '5':
			case '6': case '7': case '8':
			case '9': case '-': case ' ': break;
			case 'M': state = state_M; break;
			default: state = state_IGNORE;
		}
		break;

		case state_M:
		switch (c) {
			case ' ': break;
			case '1': state = state_M1; break;
			case '4': state = state_M4; break;
			default: state = state_IGNORE;
		}
		break;

		case state_M1:
		switch (c) {
			case '0': state = state_M10; break;
			case '1': state = state_M11; break;
			default: state = state_IGNORE;
		}
		break;

		case state_M10:
		state = (c == '8') ? state_M108 : state_IGNORE;
		break;

		case state_M11:
		state = (c == '2') ? state_M112 : state_IGNORE;
		break;

		case state_M4:
		state = (c == '1') ? state_M41 : state_IGNORE;
		break;

		case state_M41:
		state = (c == '0') ? state_M410 : state_IGNORE;
		break;

		case state_IGNORE:
		if (c == '\n') state = state_RESET;
		break;

		default:
		if (c == '\n') {
			switch (state) {
				case state_M108:
				wait_for_heatup = false;
				break;
				case state_M112:
				kill(PSTR(MSG_KILLED));
				break;
				case state_M410:
				quickstop_stepper();
				break;
				default:
				break;
			}
			state = state_RESET;
		}
	}
}
#endif
