/*
 *  ps2kbd.h      header file for PS/2 keyboard support on Raspberry Pi
 *
 *  Heavily modified from the original PS2Keyboard.h, supplied by Paul
 *  Stoffregen at PJRC.com and originally intended for the Teensy.
 *
 *  Karl Lunt   9 Aug 2015
 */

/*
  PS2Keyboard.h - PS2Keyboard library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Christian Weichel <info@32leaves.net>

  ** Mostly rewritten Paul Stoffregen <paul@pjrc.com>, June 2010
  ** Modified for use with Arduino 13 by L. Abraham Smith, <n3bah@microcompdesign.com> * 
  ** Modified for easy interrup pin assignement on method begin(datapin,irq_pin). Cuningan <cuninganreset@gmail.com> **

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


#ifndef PS2KBD_H
#define PS2KBD_H


// Every call to read() returns a single byte for each
// keystroke.  These configure what byte will be returned
// for each "special" key.  To ignore a key, use zero.
#define PS2_TAB				9
#define PS2_ENTER			13
//#define PS2_BACKSPACE			127		// why is this backspace?  backspace is 8...
#define PS2_BACKSPACE		8
#define PS2_ESC				27
#define PS2_INSERT			0
#define PS2_DELETE			127
#define PS2_HOME			128			// special non-ASCII char
#define PS2_END				129			// special non-ASCII char
#define PS2_PAGEUP			130			// special non-ASCII char
#define PS2_PAGEDOWN		131			// special non-ASCII char
#define PS2_UPARROW			132			// special non-ASCII char
#define PS2_LEFTARROW		133			// special non-ASCII char
#define PS2_DOWNARROW		134			// special non-ASCII char
#define PS2_RIGHTARROW		135			// special non-ASCII char
#define PS2_F1				0
#define PS2_F2				0
#define PS2_F3				0
#define PS2_F4				0
#define PS2_F5				0
#define PS2_F6				0
#define PS2_F7				0
#define PS2_F8				0
#define PS2_F9				0
#define PS2_F10				0
#define PS2_F11				0
#define PS2_F12				0
#define PS2_SCROLL			0

#define PS2_INVERTED_EXCLAMATION	161 // ¡
#define PS2_CENT_SIGN			162 // ¢
#define PS2_POUND_SIGN			163 // £
#define PS2_CURRENCY_SIGN		164 // ¤
#define PS2_YEN_SIGN			165 // ¥
#define PS2_BROKEN_BAR                 	166 // ¦
#define PS2_SECTION_SIGN		167 // §
#define PS2_DIAERESIS			168 // ¨
#define PS2_COPYRIGHT_SIGN		169 // ©
#define PS2_FEMININE_ORDINAL		170 // ª
#define PS2_LEFT_DOUBLE_ANGLE_QUOTE	171 // «
#define PS2_NOT_SIGN			172 // ¬
#define PS2_HYPHEN			173
#define PS2_REGISTERED_SIGN		174 // ®
#define PS2_MACRON			175 // ¯
#define PS2_DEGREE_SIGN			176 // °
#define PS2_PLUS_MINUS_SIGN		177 // ±
#define PS2_SUPERSCRIPT_TWO		178 // ²
#define PS2_SUPERSCRIPT_THREE		179 // ³
#define PS2_ACUTE_ACCENT		180 // ´
#define PS2_MICRO_SIGN			181 // µ
#define PS2_PILCROW_SIGN		182 // ¶
#define PS2_MIDDLE_DOT			183 // ·
#define PS2_CEDILLA			184 // ¸
#define PS2_SUPERSCRIPT_ONE		185 // ¹
#define PS2_MASCULINE_ORDINAL		186 // º
#define PS2_RIGHT_DOUBLE_ANGLE_QUOTE	187 // »
#define PS2_FRACTION_ONE_QUARTER	188 // ¼
#define PS2_FRACTION_ONE_HALF		189 // ½
#define PS2_FRACTION_THREE_QUARTERS	190 // ¾
#define PS2_INVERTED_QUESTION MARK	191 // ¿
#define PS2_A_GRAVE			192 // À
#define PS2_A_ACUTE			193 // Á
#define PS2_A_CIRCUMFLEX		194 // Â
#define PS2_A_TILDE			195 // Ã
#define PS2_A_DIAERESIS			196 // Ä
#define PS2_A_RING_ABOVE		197 // Å
#define PS2_AE				198 // Æ
#define PS2_C_CEDILLA			199 // Ç
#define PS2_E_GRAVE			200 // È
#define PS2_E_ACUTE			201 // É
#define PS2_E_CIRCUMFLEX		202 // Ê
#define PS2_E_DIAERESIS			203 // Ë
#define PS2_I_GRAVE			204 // Ì
#define PS2_I_ACUTE			205 // Í
#define PS2_I_CIRCUMFLEX		206 // Î
#define PS2_I_DIAERESIS			207 // Ï
#define PS2_ETH				208 // Ð
#define PS2_N_TILDE			209 // Ñ
#define PS2_O_GRAVE			210 // Ò
#define PS2_O_ACUTE			211 // Ó
#define PS2_O_CIRCUMFLEX		212 // Ô
#define PS2_O_TILDE			213 // Õ
#define PS2_O_DIAERESIS			214 // Ö
#define PS2_MULTIPLICATION		215 // ×
#define PS2_O_STROKE			216 // Ø
#define PS2_U_GRAVE			217 // Ù
#define PS2_U_ACUTE			218 // Ú
#define PS2_U_CIRCUMFLEX		219 // Û
#define PS2_U_DIAERESIS			220 // Ü
#define PS2_Y_ACUTE			221 // Ý
#define PS2_THORN			222 // Þ
#define PS2_SHARP_S			223 // ß
#define PS2_a_GRAVE			224 // à
#define PS2_a_ACUTE			225 // á
#define PS2_a_CIRCUMFLEX		226 // â
#define PS2_a_TILDE			227 // ã
#define PS2_a_DIAERESIS			228 // ä
#define PS2_a_RING_ABOVE		229 // å
#define PS2_ae				230 // æ
#define PS2_c_CEDILLA			231 // ç
#define PS2_e_GRAVE			232 // è
#define PS2_e_ACUTE			233 // é
#define PS2_e_CIRCUMFLEX		234 // ê
#define PS2_e_DIAERESIS			235 // ë
#define PS2_i_GRAVE			236 // ì
#define PS2_i_ACUTE			237 // í
#define PS2_i_CIRCUMFLEX		238 // î
#define PS2_i_DIAERESIS			239 // ï
#define PS2_eth				240 // ð
#define PS2_n_TILDE			241 // ñ
#define PS2_o_GRAVE			242 // ò
#define PS2_o_ACUTE			243 // ó
#define PS2_o_CIRCUMFLEX		244 // ô
#define PS2_o_TILDE			245 // õ
#define PS2_o_DIAERESIS			246 // ö
#define PS2_DIVISION			247 // ÷
#define PS2_o_STROKE			248 // ø
#define PS2_u_GRAVE			249 // ù
#define PS2_u_ACUTE			250 // ú
#define PS2_u_CIRCUMFLEX		251 // û
#define PS2_u_DIAERESIS			252 // ü
#define PS2_y_ACUTE			253 // ý
#define PS2_thorn			254 // þ
#define PS2_y_DIAERESIS			255 // ÿ


#define PS2_KEYMAP_SIZE 136

typedef struct
{
	uint8_t		noshift[PS2_KEYMAP_SIZE];
	uint8_t		shift[PS2_KEYMAP_SIZE];
	uint8_t		uses_altgr;
	uint8_t		altgr[PS2_KEYMAP_SIZE];
} PS2Keymap_t;



/*
 *  Available keymaps.  Pointers to these keymaps can be passed into
 *  ps2kbd_Init() to define the current locale.
 */
extern const  PS2Keymap_t		PS2Keymap_US;
extern const  PS2Keymap_t		PS2Keymap_German;
extern const  PS2Keymap_t		PS2Keymap_French;


/*
 *  ps2kbd_Init      set up the PS/2 keyboard support
 *
 *  Argument dataGPIO is the number of the GPIO pin to use for the keyboard
 *  data line.
 *
 *  Argument clkGPIO is the number of the GPIO pin to use for the keyboard
 *  clock line.
 *
 *  Argument ps2Keymap is the address of a predefined keymap for the active
 *  locale.
 *
 *  Argument ISRCallback is a pointer to a function that the calling program
 *  can invoke from within an interrupt service handler, to check and process
 *  any keyboard data.
 *
 *  The GPIO numbers for the first two arguments should be the GPIO bits used,
 *  not the pin numbers on a RasPi expansion connector.  For example, on a
 *  RasPi B, pin P1-16 is GPIO 23.  If you wire your keyboard DATA line to
 *  pin P1-16, you would pass 23 as the corresponding argument for the dataGPIO
 *  argument.
 *
 *  The callback function pointer returned by this routine should be used
 *  inside your general-purpose IRQ handler.  The bcm2835 basically has
 *  a single ISR for all interrupts (yes, I know about the FIQ).  Rather than
 *  force you to embed keyboard code in every project that uses a PS/2 keyboard,
 *  I've designed the PS/2 library to pass back a pointer to the IRQ code.
 *
 *  Assume you have saved the callback function pointer in varPS2KeyboardCallbackPtr.
 *  Inside your ISR, do this:
 *
 *  void __attribute__((interrupt("IRQ"))) interrupt_vector(void)
 *  {
 *
 *      < ... local ISR code you have to have for your project ... >
 *
 *		varPS2KeyboardCallbackPtr();		// invoke the PS/2 ISQ handler
 *
 *  }
 *
 *  The keyboard ISR handler will take care of testing and clearing flags
 *  releated to the PS/2 keyboard.
 */
void	ps2kbd_Init(uint8_t  dataGPIO,
					uint8_t  clkGPIO,
					const  PS2Keymap_t  *keymap,
					void  (**ISRCallback)(void));

/*
 *  ps2kbd_Available      returns non-zero if at least one keyboard char is available
 */
int		ps2kbd_Available(void);


/*
 *  ps2kbd_Read      returns next byte from keyboard
 *
 *  Upon exit, this routine returns the next unread byte from the keyboard.
 *  If there is no byte available, this routine returns -1.
 *
 *  The proper way to use this routine is to call ps2kbd_Available() first,
 *  to check that the keyboard buffer actually contains data.  Only call
 *  ps2kbd_Read() if the call to ps2kbd_Available() shows a char is waiting.
 */
int		ps2kbd_Read(void);


#endif
