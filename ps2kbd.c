/*
 *  ps2kbd.c      PS/2 keyboard support for the Raspberry Pi
 *
 *  This code is based on the work by Paul Stoffregen, L. Abraham Smith,
 *  and others.  I've retained Paul's copyright notice below.
 *
 *  This code assumes a connection to the Raspberry Pi's expansion
 *  connector (P1).  Two connections are required, one for the
 *  DATA signal and one for the CLK signal.
 *
 *  WARNING!  The Raspberry Pi GPIO inputs are NOT 5 VDC tolerant!
 *  You can use the 5 VDC supply on the RasPi to drive your keyboard,
 *  but you MUST use voltage dividers on the CLK and DATA lines when
 *  you wire them to your RasPi, or you will damage your RasPi.
 *
 *  Here is a diagram for suitable voltage dividers:
 *
 *  KBD DATA <---------[ 2.2K ]------+-------[ 3.3K ]---------> GND
 *                                   |
 *                                   +------------------------> RasPi GPIO
 *
 *  KBD CLK  <---------[ 2.2K ]------+-------[ 3.3K ]---------> GND
 *                                   |
 *                                   +------------------------> RasPi GPIO
 *
 *  Karl Lunt (10 Aug 2015)
 */

/*
  PS2Keyboard.cpp - PS2Keyboard library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Christian Weichel <info@32leaves.net>

  ** Mostly rewritten Paul Stoffregen <paul@pjrc.com> 2010, 2011
  ** Modified for use beginning with Arduino 13 by L. Abraham Smith, <n3bah@microcompdesign.com> * 
  ** Modified for easy interrup pin assignement on method begin(datapin,irq_pin). Cuningan <cuninganreset@gmail.com> **

  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/PS2Keyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html

  Version 2.4 (March 2013)
  - Support Teensy 3.0, Arduino Due, Arduino Leonardo & other boards
  - French keyboard layout, David Chochoi, tchoyyfr at yahoo dot fr

  Version 2.3 (October 2011)
  - Minor bugs fixed

  Version 2.2 (August 2011)
  - Support non-US keyboards - thanks to Rainer Bruch for a German keyboard :)

  Version 2.1 (May 2011)
  - timeout to recover from misaligned input
  - compatibility with Arduino "new-extension" branch
  - TODO: send function, proposed by Scott Penrose, scooterda at me dot com

  Version 2.0 (June 2010)
  - Buffering added, many scan codes can be captured without data loss
    if your sketch is busy doing other work
  - Shift keys supported, completely rewritten scan code to ascii
  - Slow linear search replaced with fast indexed table lookups
  - Support for Teensy, Arduino Mega, and Sanguino added

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdint.h>
#include "bcm2835.h"
#include "systimer.h"
#include "ps2kbd.h"


#ifndef  false
#define  false  0
#define  true  !false
#endif


#define BUFFER_SIZE 45
static volatile uint8_t			buffer[BUFFER_SIZE];
static volatile uint8_t			head;
static volatile uint8_t			tail;
static uint8_t					CharBuffer=0;
static uint8_t					UTF8next=0;
static const PS2Keymap_t		*keymap=0;

static volatile uint32_t		*EventDetStatusReg;
static volatile uint32_t		*InputReg;
static uint32_t					clkPinMask;
static uint32_t					dataPinMask;


/*
 *  Local functions
 */
static char						ps2kbd_GetISO8859Code(void);
static inline uint8_t			get_scan_code(void);



/*
 *  ps2kbd_Callback      handler for PS/2 keyboard GPIO interrupts
 */

void ps2kbd_Callback(void)
{
	static uint8_t		bitcount=0;
	static uint8_t		incoming=0;
	static uint32_t		prev_ms=0;
	uint32_t			now_ms;
	uint8_t				n;
	uint32_t			val;

	if ((*EventDetStatusReg & clkPinMask) == 0)  return;	// if no edge, leave now
	*EventDetStatusReg = clkPinMask;		// first off, clear the GPIO interrupt flag
	
	now_ms = SysTimer_GetMillisecs();
	if (now_ms - prev_ms > 250)
	{
		bitcount = 0;
		incoming = 0;
	}

	val = *InputReg & dataPinMask;
	if (val)  val = 1;

	prev_ms = now_ms;
	n = bitcount - 1;
	if (n <= 7)
	{
		incoming |= (val << n);
	}
	bitcount++;
	if (bitcount == 11)
	{
		uint8_t i = head + 1;
		if (i >= BUFFER_SIZE) i = 0;
		if (i != tail)
		{
			buffer[i] = incoming;
			head = i;
		}
		bitcount = 0;
		incoming = 0;
	}
}



static inline uint8_t get_scan_code(void)
{
	uint8_t c, i;

	i = tail;
	if (i == head) return 0;
	i++;
	if (i >= BUFFER_SIZE) i = 0;
	c = buffer[i];
	tail = i;
	return c;
}


#define BREAK     0x01
#define MODIFIER  0x02
#define SHIFT_L   0x04
#define SHIFT_R   0x08
#define ALTGR     0x10

static char ps2kbd_GetISO8859Code(void)
{
	static uint8_t state=0;
	uint8_t s;
	char c;

	while (1)
	{
		s = get_scan_code();
		if (!s) return 0;
		if (s == 0xF0)
		{
			state |= BREAK;
		}
		else if (s == 0xE0)
		{
			state |= MODIFIER;
		}
		else
		{
			if (state & BREAK)
			{
				if (s == 0x12)
				{
					state &= ~SHIFT_L;
				}
				else if (s == 0x59)
				{
					state &= ~SHIFT_R;
				}
				else if (s == 0x11 && (state & MODIFIER))
				{
					state &= ~ALTGR;
				}
				// CTRL, ALT & WIN keys could be added
				// but is that really worth the overhead?
				state &= ~(BREAK | MODIFIER);
				continue;
			}
			if (s == 0x12)
			{
				state |= SHIFT_L;
				continue;
			}
			else if (s == 0x59)
			{
				state |= SHIFT_R;
				continue;
			}
			else if (s == 0x11 && (state & MODIFIER))
			{
				state |= ALTGR;
			}
			c = 0;
			if (state & MODIFIER)
			{
				switch (s)
				{
				  case 0x70: c = PS2_INSERT;      break;
				  case 0x6C: c = PS2_HOME;        break;
				  case 0x7D: c = PS2_PAGEUP;      break;
				  case 0x71: c = PS2_DELETE;      break;
				  case 0x69: c = PS2_END;         break;
				  case 0x7A: c = PS2_PAGEDOWN;    break;
				  case 0x75: c = PS2_UPARROW;     break;
				  case 0x6B: c = PS2_LEFTARROW;   break;
				  case 0x72: c = PS2_DOWNARROW;   break;
				  case 0x74: c = PS2_RIGHTARROW;  break;
				  case 0x4A: c = '/';             break;
				  case 0x5A: c = PS2_ENTER;       break;
				  case 0x66: c = PS2_BACKSPACE;   break;	// added for backspace  KEL
				  default: break;
				}
			}
			else if ((state & ALTGR) && keymap->uses_altgr)
			{
				if (s < PS2_KEYMAP_SIZE)
					c = *(keymap->altgr + s);
			}
			else if (state & (SHIFT_L | SHIFT_R))
			{
				if (s < PS2_KEYMAP_SIZE)
					c = *(keymap->shift + s);
			}
			else
			{
				if (s < PS2_KEYMAP_SIZE)
					c = *(keymap->noshift + s);
			}
			state &= ~(BREAK | MODIFIER);
			if (c) return c;
		}
	}
}



int  ps2kbd_Available(void)
{
//	if (head == tail)  return false;
	if (CharBuffer || UTF8next) return true;
	CharBuffer = ps2kbd_GetISO8859Code();
	if (CharBuffer) return true;
	return false;
}



int  ps2kbd_Read()
{
	uint8_t result;

	result = UTF8next;
	if (result)
	{
		UTF8next = 0;
	}
	else
	{
		result = CharBuffer;
		if (result)
		{
			CharBuffer = 0;
		}
		else
		{
			result = ps2kbd_GetISO8859Code();
		}
		if (result >= 128)
		{
			UTF8next = (result & 0x3F) | 0x80;
			result = ((result >> 6) & 0x1F) | 0xC0;
		}
	}
	if (!result) return -1;
	return result;
}




void  ps2kbd_Init(uint8_t  dataGPIO,
				 uint8_t  clkGPIO,
				 const PS2Keymap_t  *map,
				 void  (**Callback)(void))
{
	uint32_t			mask;

	clkPinMask = (1 << (clkGPIO % 32));
	dataPinMask = (1 << (dataGPIO % 32));

/*
 *  First, set the flags in the GPIO module so a falling edge
 *  on the CLK pin will set an interrupt flag.  The register
 *  for this is GPFEN0 for GPIO bits 0-31 or GPENF1 for GPIO
 *  bits 32-53.
 */
	if (clkGPIO < 32)
	{
		EventDetStatusReg = &(BCM2835_GPIO->GPEDS0);
		BCM2835_GPIO->GPFEN0 |= clkPinMask;

	}
	else
	{
		EventDetStatusReg = &(BCM2835_GPIO->GPEDS1);
		BCM2835_GPIO->GPFEN1 |= clkPinMask;
	}

/*
 *  The appropriate bit in the GPIO Event Detect Status Register
 *  GPEDS0 or GPEDS1) will be set whenever the CLK input goes low.
 *  To be safe, clear that bit now by writing a 1 to it.
 */
	*EventDetStatusReg = clkPinMask;

/*
 *  Next, set the proper bit in the IRQ module so an IRQ interrupt
 *  fires when the CLK pin goes low.  The register for this is
 *  IRQ_ENABLE2.
 *
 *  Unfortunately, the BCM2835 ARM Peripherals manual sucks.  The
 *  table listing the interrupts from the ARM peripherals includes
 *  the following GPIO interrupt information:
 *
 *  IRQ 49 is gpio_int[0]
 *  IRQ 50 is gpio_int[1]
 *  IRQ 51 is gpio_int[2]
 *  IRQ 52 is gpio_int[3]
 *
 *  However, this is the only place in the document where the string
 *  "gpio_int" appears.  No clue as to what the four subsets of gpio_int
 *  mean or how to tell to which subset a GPIO pin belongs.
 *
 *  By experimentation, I discovered that if clkGPIO is 24, the GPIO
 *  interrupt flag I need is IRQ 49.  No idea why that is the case.
 *  The following code uses a /32 to arrive at a shift value for
 *  creating an IRQ mask, but it could well be wrong.  If you use
 *  a clkGPIO other than 24 and this code breaks, dork around with
 *  other flags until you find one that works.  Sorry.
 */
	mask = 1 << ((clkGPIO / 32) + 17);
	BCM2835_IRQ->IRQ_ENABLE2 = mask;

	if (dataGPIO < 32)
	{
		InputReg = &(BCM2835_GPIO->GPLEV0);
	}
	else
	{
		InputReg = &(BCM2835_GPIO->GPLEV1);
	}

	keymap = map;
	head = 0;
	tail = 0;

	*Callback = &ps2kbd_Callback;
}



// http://www.quadibloc.com/comp/scan.htm
// http://www.computer-engineering.org/ps2keyboard/scancodes2.html

// These arrays provide a simple key map, to turn scan codes into ISO8859
// output.  If a non-US keyboard is used, these may need to be modified
// for the desired output.
//

const PS2Keymap_t PS2Keymap_US =
{
  // without shift
	{	0, PS2_F9,	0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,		// 0x00 - 0x07
		0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,		// 0x08 - 0x0f
		0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,	// 0x10 - 0x17
		0, 0, 'z', 's', 'a', 'w', '2', 0,							// 0x18 - 0x1f
		0, 'c', 'x', 'd', 'e', '4', '3', 0,							// 0x20 - 0x27
		0, ' ', 'v', 'f', 't', 'r', '5', 0,							// 0x28 - 0x2f
		0, 'n', 'b', 'h', 'g', 'y', '6', 0,							// 0x30 - 0x37
		0, 0, 'm', 'j', 'u', '7', '8', 0,							// 0x38 - 0x3f
		0, ',', 'k', 'i', 'o', '0', '9', 0,							// 0x40 - 0x47
		0, '.', '/', 'l', ';', 'p', '-', 0,							// 0x48 - 0x4f
		0, 0, '\'', 0, '[', '=', 0, 0,								// 0x50 - 0x57
		0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER, ']', 0, '\\', 0, 0,	// 0x58 - 0x5f
		0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,							// 0x60 - 0x67
		0, '1', 0, '4', '7', 0, 0, 0,								// 0x68 - 0x6f
		'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,		// 0x70 - 0x77
		PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,			// 0x78 - 0x7f
		0, 0, 0, PS2_F7 },											// 0x80 - 0x83
  // with shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '~', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
	0, 0, 'Z', 'S', 'A', 'W', '@', 0,
	0, 'C', 'X', 'D', 'E', '$', '#', 0,
	0, ' ', 'V', 'F', 'T', 'R', '%', 0,
	0, 'N', 'B', 'H', 'G', 'Y', '^', 0,
	0, 0, 'M', 'J', 'U', '&', '*', 0,
	0, '<', 'K', 'I', 'O', ')', '(', 0,
	0, '>', '?', 'L', ':', 'P', '_', 0,
	0, 0, '"', 0, '{', '+', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '}', 0, '|', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
	0
};


const PS2Keymap_t PS2Keymap_German =
{
  // without shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '^', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
	0, 0, 'y', 's', 'a', 'w', '2', 0,
	0, 'c', 'x', 'd', 'e', '4', '3', 0,
	0, ' ', 'v', 'f', 't', 'r', '5', 0,
	0, 'n', 'b', 'h', 'g', 'z', '6', 0,
	0, 0, 'm', 'j', 'u', '7', '8', 0,
	0, ',', 'k', 'i', 'o', '0', '9', 0,
	0, '.', '-', 'l', PS2_o_DIAERESIS, 'p', PS2_SHARP_S, 0,
	0, 0, PS2_a_DIAERESIS, 0, PS2_u_DIAERESIS, '\'', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '+', 0, '#', 0, 0,
	0, '<', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
  // with shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, PS2_DEGREE_SIGN, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
	0, 0, 'Y', 'S', 'A', 'W', '"', 0,
	0, 'C', 'X', 'D', 'E', '$', PS2_SECTION_SIGN, 0,
	0, ' ', 'V', 'F', 'T', 'R', '%', 0,
	0, 'N', 'B', 'H', 'G', 'Z', '&', 0,
	0, 0, 'M', 'J', 'U', '/', '(', 0,
	0, ';', 'K', 'I', 'O', '=', ')', 0,
	0, ':', '_', 'L', PS2_O_DIAERESIS, 'P', '?', 0,
	0, 0, PS2_A_DIAERESIS, 0, PS2_U_DIAERESIS, '`', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '*', 0, '\'', 0, 0,
	0, '>', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
	1,
  // with altgr
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, 0, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, '@', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_SUPERSCRIPT_TWO, 0,
	0, 0, 0, 0, PS2_CURRENCY_SIGN, 0, PS2_SUPERSCRIPT_THREE, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, PS2_MICRO_SIGN, 0, 0, '{', '[', 0,
	0, 0, 0, 0, 0, '}', ']', 0,
	0, 0, 0, 0, 0, 0, '\\', 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '~', 0, '#', 0, 0,
	0, '|', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 }
};


const PS2Keymap_t PS2Keymap_French =
{
  // without shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, PS2_SUPERSCRIPT_TWO, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'a', '&', 0,
	0, 0, 'w', 's', 'q', 'z', PS2_e_ACUTE, 0,
	0, 'c', 'x', 'd', 'e', '\'', '"', 0,
	0, ' ', 'v', 'f', 't', 'r', '(', 0,
	0, 'n', 'b', 'h', 'g', 'y', '-', 0,
	0, 0, ',', 'j', 'u', PS2_e_GRAVE, '_', 0,
	0, ';', 'k', 'i', 'o', PS2_a_GRAVE, PS2_c_CEDILLA, 0,
	0, ':', '!', 'l', 'm', 'p', ')', 0,
	0, 0, PS2_u_GRAVE, 0, '^', '=', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '$', 0, '*', 0, 0,
	0, '<', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
  // with shift
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, 0, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'A', '1', 0,
	0, 0, 'W', 'S', 'Q', 'Z', '2', 0,
	0, 'C', 'X', 'D', 'E', '4', '3', 0,
	0, ' ', 'V', 'F', 'T', 'R', '5', 0,
	0, 'N', 'B', 'H', 'G', 'Y', '6', 0,
	0, 0, '?', 'J', 'U', '7', '8', 0,
	0, '.', 'K', 'I', 'O', '0', '9', 0,
	0, '/', PS2_SECTION_SIGN, 'L', 'M', 'P', PS2_DEGREE_SIGN, 0,
	0, 0, '%', 0, PS2_DIAERESIS, '+', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, PS2_POUND_SIGN, 0, PS2_MICRO_SIGN, 0, 0,
	0, '>', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 },
	1,
  // with altgr
	{0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, 0, 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, '@', 0, 0,
	0, 0, 0, 0, 0, 0, '~', 0,
	0, 0, 0, 0, 0 /*PS2_EURO_SIGN*/, '{', '#', 0,
	0, 0, 0, 0, 0, 0, '[', 0,
	0, 0, 0, 0, 0, 0, '|', 0,
	0, 0, 0, 0, 0, '`', '\\', 0,
	0, 0, 0, 0, 0, '@', '^', 0,
	0, 0, 0, 0, 0, 0, ']', 0,
	0, 0, 0, 0, 0, 0, '}', 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, PS2_CURRENCY_SIGN, 0, '#', 0, 0,
	0, '|', 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 }
};

