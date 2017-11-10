#include "lcd_engine.h"
#include <avr/io.h>
#include <stdlib.h>
#include "connections.h"
#include "macros.h"
#include <util/delay.h>

//Declare FONT
static unsigned char ASCII5_7[];

//***************************************************
//					Init LCD_engine			
//***************************************************
//Creates and allocates memory for the framebuffer and inits the display
LCD_engine * init_lcd(void) {
	LCD_engine *me = malloc(sizeof(LCD_engine));
	pixelSetOrClearOrInvert(me, SET_PIXELS);
	LCD_init();
	return me;
}

//This will just return a new framebuffer, used when you like to have more then one, for ex; ingame meny
LCD_engine * get_new_ram(void) {
	LCD_engine *me = malloc(sizeof(LCD_engine));
	pixelSetOrClearOrInvert(me, SET_PIXELS);

	return me;
}
//Rellocate memory for the buffer
void trash_ram(LCD_engine * me) {
	free(me);
}

void LCD_init(void) {

	//Init our SPI so we can talk with the LCD
	SPI_MasterInit();
	//Init the LCD
	DOG_MasterInit();
	//Start LCD
	PORT_RE &= ~(1 << OUT_RE); // RES\ = LOW 
	_delay_ms(50);
	PORT_RE |= (1 << OUT_RE); // RES\ = HIGH
	_delay_ms(50);
	//Send init commands to LCD
	initDOGL128();
	//Clear screen, random pixles on now
	LCD_CLR();
	DOGENABLE; //Make sure it's activated, this is a macro, se macros.h
}

void DOG_MasterInit(void) {
	//Direction 
	DDR_SPI |= (1 << DD_A0) | (1 << DD_RE); //A0 and RE is out 
	//Data
	//PORT_CS&=~(1<<OUT_CS); //Chip select to 0 
	PORT_RE |= (1 << OUT_RE); //Reset to 1
	PORT_A0 |= (1 << OUT_A0); //A0 (command or data) to 1
}

void SPI_MasterInit(void) {
	// Set MOSI and SCK output, all others input
	DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK) | (1 << DD_CS);

	// Enable SPI, Master, set clock rate fck/16 it can handle much faster, but no need. The time to fade the pixles is whats dragging it all down
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}
//Our way to talk to the LCD, low level
void SPI_MasterTransmit(char cData) {
	// Start transmission 
	SPDR = cData;
	// Wait for transmission complete, maby this should be before, now it may unnecessary block. But it would not make big diffrent
	while (!(SPSR & (1 << SPIF))) {
	}
}

void initDOGL128(void) {
	DOGENABLE; //Enable LCD (Chip select)
	DOGCOMMAND; //We are sending commands, setting A0 low
	//From EA DOGL128 documentation, defined in connections.h
	SPI_MasterTransmit(DISPSTART + 0);
	SPI_MasterTransmit( ADCREVERSE);
	SPI_MasterTransmit( COMNORMAL);
	SPI_MasterTransmit( DISPNORMAL);
	SPI_MasterTransmit( LCDBIAS9);
	SPI_MasterTransmit(SETPOWERCTRL + 7);
	SPI_MasterTransmit( BOOSTERRATIO);
	SPI_MasterTransmit(0);
	SPI_MasterTransmit(REGRESISTOR + 7);
	SPI_MasterTransmit( SETCONTRAST);
	SPI_MasterTransmit( CONTRAST_VALUE);
	SPI_MasterTransmit( STATINDMODE);
	SPI_MasterTransmit(0);
	SPI_MasterTransmit( DISPON);
	DOGDISABLE; //Why? Just make sure to enable it later


}

//***************************************************
//					LCD drawing			
//***************************************************

//Swaap them, used for line algoritm
void swaap(int *i, int *j) {
	int t = *i;
	*i = *j;
	*j = t;
}
//Sett if we want to clear, set or invert pixels
void pixelSetOrClearOrInvert(LCD_engine * me, char set) {
	me->SETPIXEL = set;
}
//Set if we want all the pixels inverted, done by command to LCD
void DisplaySetDispInverted(LCD_engine * me, char set) {
	me->DISPINVERTED = set;// 1 for inverted, 0 for normal
	DOGCOMMAND;
	if (set) {
		SPI_MasterTransmit( DISPREVERSE);
	} else {
		SPI_MasterTransmit( DISPNORMAL);
	}
}
//Set dubbelbuffring. Always used in my examples, but no need for it, just make sure we only send changed pixles, it's a lot faster.
void setDubbelbuffer(LCD_engine * me, char dubbel) {
	me->DUBBELBUFFER = dubbel;
}

//Draw a predefined 16x16 px image to the buffer
void LCD_draw16x16ImageToRam(LCD_engine * me, unsigned char image[32],
		unsigned char x, unsigned char y) {

	for (int i = 0; i < 16; i++) {
		if (y % 8 != 0) { //Not a multiple of 8, we need to break up the image
			me->LCD_RAM[((y / 8)) * 128 + (i + x)] |= image[(i)] << (y % 8);
			me->LCD_RAM[((y / 8) + 1) * 128 + (i + x)] |= image[(i)] >> (8 - y
					% 8);
			me->LCD_RAM[((y / 8) + 1) * 128 + (i + x)] |= image[(i + 16)] << (y
					% 8);
			me->LCD_RAM[((y / 8) + 2) * 128 + (i + x)] |= image[(i + 16)] >> (8
					- y % 8);
		} else {
			me->LCD_RAM[((y / 8)) * 128 + (i + x)] |= image[(i)];
			me->LCD_RAM[((y / 8) + 1) * 128 + (i + x)] |= image[(i + 16)];
		}

	}

	//Say whats new pixles for dubblebuffring
	if (y % 8 != 0) {
		me->LCD_changed_RAM[(((y) / 8 + 2) * 128 + x) / 8] = 0xFF;
		me->LCD_changed_RAM[(((y) / 8 + 2) * 128 + x) / 8 + 1] = 0xFF;
	}

	me->LCD_changed_RAM[((y / 8) * 128 + x) / 8] = 0xFF;
	me->LCD_changed_RAM[((y / 8) * 128 + x) / 8 + 1] = 0xFF;
	me->LCD_changed_RAM[(((y) / 8 + 1) * 128 + x) / 8] = 0xFF;
	me->LCD_changed_RAM[(((y) / 8 + 1) * 128 + x) / 8 + 1] = 0xFF;

}

//Draw a fullscreen image to buffer
void LCD_drawImageToRam(LCD_engine * me, unsigned char image[1024]) {
	for (int i = 0; i < 1024; i++) {
		me->LCD_RAM[i] = image[i];
	}
	for (int j = 0; j < 128; j++) {
		me->LCD_changed_RAM[j] = 0xFF;
	}
}

//Draw a single pixel to buffer, this needs to be fast, and it is
void LCD_drawPixel(LCD_engine * me, int x, int y) {
	if (x > 127 || y > 63 || x < 0 || y < 0) {//Not a valid pixel
		return;
	}
	if (me->SETPIXEL == SET_PIXELS) {//set pixel
		me->LCD_RAM[(y / 8) * 128 + x] |= 1 << (y % 8); //Set
	} else if (me->SETPIXEL == CLEAR_PIXELS) {//clear pixel
		me->LCD_RAM[(y / 8) * 128 + x] &= ~(1 << (y % 8)); //Clear
	} else {//INVERT_PIXELS allways go to this when out of range, for debugging
		if (me->LCD_RAM[(y / 8) * 128 + x] & (1 << (y % 8))) {//is it set?

			me->LCD_RAM[(y / 8) * 128 + x] &= ~(1 << (y % 8)); //Clear
		} else {
			me->LCD_RAM[(y / 8) * 128 + x] |= 1 << (y % 8); //Set
		}

	}

	if (me->DUBBELBUFFER) {//Say where we have changed, this sets one bit
		me->LCD_changed_RAM[((y / 8) * 128 + x) / 8] |= 1 << (7 - ((y / 8)
				* 128 + x) % 8);
		return;
	}
	//No dubbelbuffring, just print it out now
	LCD_setColumn(x);
	LCD_setPage(y / 8);
	DOGDATA;
	SPI_MasterTransmit(me->LCD_RAM[(y / 8) * 128 + x]);
}
//Redraw the whole RAM (screenbuffer) to LCD
void redrawRAM(LCD_engine * me) {
	for (int adrPage = 0; adrPage < 8; adrPage++) {
		//Set where
		DOGCOMMAND;
		SPI_MasterTransmit(PAGEADR + adrPage);
		SPI_MasterTransmit(COLADRH + 0);
		SPI_MasterTransmit(COLADRL + 0);
		DOGDATA;
		for (int adrLaw = 0; adrLaw < 128; adrLaw++) {
			SPI_MasterTransmit(me->LCD_RAM[adrPage * 128 + adrLaw]); //Send
		}
	}

}
//When using dubbelbuffring, this only sends out changed pixels 
void pushRAM(LCD_engine * me) {
	int active_col = 0;
	for (int adrPage = 0; adrPage < 8; adrPage++) {

		DOGCOMMAND;
		SPI_MasterTransmit(PAGEADR + adrPage);
		SPI_MasterTransmit(COLADRH + 0);
		SPI_MasterTransmit(COLADRL + 0);
		active_col = -1; //So we dont send "change column" when not needed, it takes time if we do
		DOGDATA;
		for (int adrLaw = 0; adrLaw < 128; adrLaw++) {
			if (me->LCD_changed_RAM[((adrPage) * 128 + adrLaw) / 8] & (1 << (7
					- ((adrPage) * 128 + adrLaw) % 8))) {//pixel have changed

				if (active_col != adrLaw) {//Dont send column data if its the next one (its auto incremented by the LCD)
					LCD_setColumn(adrLaw);
					active_col = adrLaw;
					DOGDATA;
				}
				active_col++;//Auto incremented by LCD
				//Send data
				SPI_MasterTransmit(me->LCD_RAM[adrPage * 128 + adrLaw]);
				//Set this bit to 0, its up to date
				me->LCD_changed_RAM[((adrPage) * 128 + adrLaw) / 8] &= ~(1
						<< (7 - ((adrPage) * 128 + adrLaw) % 8));

			}

		}
	}
}

//This is 'Bresenhams line algoritm', ported from psuedo code.
void LCD_drawLine(LCD_engine * me, int x0, int y0, int x1, int y1) {
	//http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	int steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swaap(&x0, &y0);
		swaap(&x1, &y1);
	}
	if (x0 > x1) {
		swaap(&x0, &x1);
		swaap(&y0, &y1);
	}
	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = deltax / 2;
	int ystep;
	int y = y0;
	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			LCD_drawPixel(me, y, x);
		} else {
			LCD_drawPixel(me, x, y);
		}
		error = error - deltay;
		if (error < 0) {
			y = y + ystep;
			error = error + deltax;
		}
	}

}

//----------------------------------------------------
//						LCD FIGURES
//----------------------------------------------------

//Fill a rect by drawing lines
void fillRect(LCD_engine * me, int x, int y, int w, int h) {
	if (w > h) {
		for (int j = 0; j < h; j++)
			LCD_drawLine(me, x, j + y, x + w, j + y);
	} else {
		for (int j = 0; j < w; j++)
			LCD_drawLine(me, x + j, y, x + j, y + h);
	}

}

//Fill a rect by drawing horizontal lines with 'inc' spacing
void h_fillRect(LCD_engine * me, int x, int y, int w, int h, int inc) {
	drawRect(me, x, y, w, h);
	for (int j = inc; j < h; j += inc)
		LCD_drawLine(me, x + 1, j + y, x + w - 1, j + y);
}

//Fill a rect by drawing vertical lines with 'inc' spacing
void v_fillRect(LCD_engine * me, int x, int y, int w, int h, int inc) {
	drawRect(me, x, y, w, h);
	for (int j = inc; j < w; j += inc)
		LCD_drawLine(me, x + j, y + 1, x + j, h + y - 1);
}

//Fill a rect by drawing horizontal and vertival lines with 'inc' spacing, like a chess board
void c_fillRect(LCD_engine * me, int x, int y, int w, int h, int inc) {
	drawRect(me, x, y, w, h);
	for (int j = inc; j < w; j += inc)
		LCD_drawLine(me, x + j, y + 1, x + j, h + y - 1);
	for (int j = inc; j < h; j += inc)
		LCD_drawLine(me, x + 1, j + y, x + w - 1, j + y);
}

//Draw a rect outline
void drawRect(LCD_engine * me, int x, int y, int w, int h) {
	LCD_drawLine(me, x + 1, y, x + w - 1, y);
	LCD_drawLine(me, x + 1, y + h, x + w - 1, y + h);
	LCD_drawLine(me, x, y, x, y + h);
	LCD_drawLine(me, x + w, y, x + w, y + h);
}

//Fill a Circel, modified 'Midpoint circle algorithm' by me, ported from psuedo code.
void fillCircle(LCD_engine * me, int x0, int y0, int radius) {

	//http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	LCD_drawLine(me, x0, y0 + radius, x0, y0 - radius);
	LCD_drawLine(me, x0 + radius, y0, x0 - radius, y0);
	while (x < y) {
		// ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		LCD_drawLine(me, x0 + x, y0 + y, x0 - x, y0 + y);
		LCD_drawLine(me, x0 + x, y0 - y, x0 - x, y0 - y);
		LCD_drawLine(me, x0 + y, y0 + x, x0 - y, y0 + x);
		LCD_drawLine(me, x0 + y, y0 - x, x0 - y, y0 - x);

	}
}
//Fill a Circel, 'Midpoint circle algorithm', basically modified 'Bresenhams line algoritm', ported from psuedo code.
void drawCircle(LCD_engine * me, int x0, int y0, int radius) {

	//http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	LCD_drawPixel(me, x0, y0 + radius);
	LCD_drawPixel(me, x0, y0 - radius);
	LCD_drawPixel(me, x0 + radius, y0);
	LCD_drawPixel(me, x0 - radius, y0);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		LCD_drawPixel(me, x0 + x, y0 + y);
		LCD_drawPixel(me, x0 - x, y0 + y);
		LCD_drawPixel(me, x0 + x, y0 - y);
		LCD_drawPixel(me, x0 - x, y0 - y);
		LCD_drawPixel(me, x0 + y, y0 + x);
		LCD_drawPixel(me, x0 - y, y0 + x);
		LCD_drawPixel(me, x0 + y, y0 - x);
		LCD_drawPixel(me, x0 - y, y0 - x);
	}
}
//		FIGURES - END

//----------------------------------------------------
//						LCD COMMANDS
//----------------------------------------------------

//Set contast
void LCD_setContrast(LCD_engine * me, unsigned char contrast) {
	DOGCOMMAND;
	SPI_MasterTransmit( SETCONTRAST);
	SPI_MasterTransmit(contrast);

}
//Clear the buffer
void LCD_RAM_CLR(LCD_engine * me) {
	for (int i = 0; i < 1024; i++) {
		me->LCD_changed_RAM[(i) / 8] = 0;
		me->LCD_RAM[i] = 0x00;

	}

}

//Clear the whole LCD
void LCD_CLR(void) {

	uint8_t adrPage, adrLaw, Data;
	Data = 0;
	DOGENABLE;

	for (adrPage = 0; adrPage < 8; adrPage++) {
		DOGCOMMAND;
		SPI_MasterTransmit(PAGEADR + adrPage);
		SPI_MasterTransmit(COLADRH + 0);
		SPI_MasterTransmit(COLADRL + 0);
		DOGDATA;
		for (adrLaw = 0; adrLaw < 128; adrLaw++) {
			SPI_MasterTransmit(Data);

		}
	}
	DOGDISABLE;
}
void resetConnection() {
	DOGENABLE;
}
void LCD_setPage(unsigned char page) {
	DOGCOMMAND;
	SPI_MasterTransmit(PAGEADR + page);
	//SPI_MasterTransmit(COLADRH+0x7);
	//SPI_MasterTransmit(COLADRL+0xF);
	//DOGDATA;


}
void LCD_setColumn(unsigned char col) {
	DOGCOMMAND;
	//SPI_MasterTransmit(PAGEADR+page);
	SPI_MasterTransmit(COLADRH + ((col & 0xF0) >> 4));
	SPI_MasterTransmit(COLADRL + (col & 0x0F));
	//DOGDATA;

}

//----------------------------------------------------
//						LCD PRINTING
//----------------------------------------------------

void printHart() {//For fun
	DOGDATA;
	SPI_MasterTransmit(0b00001100); //-##-##-
	SPI_MasterTransmit(0b00011110); //#######
	SPI_MasterTransmit(0b00111110); //#######
	SPI_MasterTransmit(0b01111100); //-#####-
	SPI_MasterTransmit(0b00111110); //--###--
	SPI_MasterTransmit(0b00011110); //---#---
	SPI_MasterTransmit(0b00001100); //-------
}

//This is from a opensource example on AVRFREAKS.COM, but a bit modified
//Prints strait to LCD, raw printing
void rprintfChar(unsigned char c) {
	DOGDATA;
	if (c > 127) {//end of table
	//Make a hart
		printHart();
		SPI_MasterTransmit(0x00); // small spaceing
		return;
	}
	if (c < 0x20)
		c = 0x20;
	unsigned long dptr = (c - 0x20) * 5; //Really needed to be a long? Maby int is enaugh?
	for (int i = 0; i < 5; i++) {
		SPI_MasterTransmit(ASCII5_7[dptr]);
		dptr = dptr + 1;
	}
	SPI_MasterTransmit(0x00); // small spaceing
}

//Printing to LCD buffer
void printfChar(LCD_engine * me, unsigned char c, unsigned char *x,
		unsigned char y) {
	DOGDATA;

	if (c > 127) {//end of table
	//No hart here
		return;
	}
	if (c < 0x20)
		c = 0x20;
	unsigned long dptr = (c - 0x20) * 5;

	for (int i = 0; i < 5; i++) {
		for (int j = 7; j >= 0; j--) {
			if (ASCII5_7[dptr] & (1 << j)) {
				LCD_drawPixel(me, *x, y + j);//This can be much more optimized, for ex, by using the same way as printing a 16x16 image. But this 
				//was done first. No time to change it now.
			}
		}
		(*x) = (*x) + 1;
		dptr = dptr + 1;
	}
}
//Print a int
void printfInt(LCD_engine * me, int i, unsigned char x, unsigned char y) {
	char str[10];
	printfStr(me, itoa(i, str, 10), x, y);
}
//Print a variable binary
void printfBin(LCD_engine * me, int i, unsigned char x, unsigned char y) {
	char str[10];
	printfStr(me, itoa(i, str, 2), x, y);
}
//Print a string
void printfStr(LCD_engine * me, char str[], unsigned char x, unsigned char y) {
	// send a string stored in RAM
	// check to make sure we have a good pointer
	if (!str)
		return;

	// print the string until a null-terminator
	while (*str) {
		printfChar(me, *str++, &x, y);
		x++;
	}
}
//Printing directly to LCD, overriding dubbelbuffring
//Print int
void rprintfInt(int i) {
	char str[10];
	rprintfStr(itoa(i, str, 10));
}
//Print a variable binary
void rprintfBin(int i) {
	char str[10];
	rprintfStr(itoa(i, str, 2));
}
//Print a string
void rprintfStr(char str[]) {
	// Check to make sure we have a good pointer
	if (!str)
		return;

	// print the string until a null-terminator
	while (*str)
		rprintfChar(*str++);
}
//Se how long the string are, for centering text.
int StrLength(char str[]) {
	int l = 0;
	if (!str)
		return -1;
	// check the string until a null-terminator
	while (*str) {
		*str++;
		l++;
	}
	return l;
}
//Center a text and print it
void rprintfStrCenter(char str[]) {
	LCD_setColumn(64 - StrLength(str) * 3);
	rprintfStr(str);
}

//----------------------------------------------------
//						LCD FONT
//----------------------------------------------------
//Maby it should be in another file, but it is only needed here

static unsigned char ASCII5_7[] = { 0x00, 0x00, 0x00, 0x00, 0x00,// (space)   
		0x00, 0x00, 0x5F, 0x00, 0x00,// !         
		0x00, 0x07, 0x00, 0x07, 0x00,// "         
		0x14, 0x7F, 0x14, 0x7F, 0x14,// #         
		0x24, 0x2A, 0x7F, 0x2A, 0x12,// $         
		0x23, 0x13, 0x08, 0x64, 0x62,// %         
		0x36, 0x49, 0x55, 0x22, 0x50,// &         
		0x00, 0x05, 0x03, 0x00, 0x00,// '         
		0x00, 0x1C, 0x22, 0x41, 0x00,// (         
		0x00, 0x41, 0x22, 0x1C, 0x00,// )         
		0x08, 0x2A, 0x1C, 0x2A, 0x08,// *         
		0x08, 0x08, 0x3E, 0x08, 0x08,// +         
		0x00, 0x50, 0x30, 0x00, 0x00,// ,         
		0x08, 0x08, 0x08, 0x08, 0x08,// -         
		0x00, 0x60, 0x60, 0x00, 0x00,// .         
		0x20, 0x10, 0x08, 0x04, 0x02,// /         
		0x3E, 0x51, 0x49, 0x45, 0x3E,// 0         
		0x00, 0x42, 0x7F, 0x40, 0x00,// 1         
		0x42, 0x61, 0x51, 0x49, 0x46,// 2         
		0x21, 0x41, 0x45, 0x4B, 0x31,// 3         
		0x18, 0x14, 0x12, 0x7F, 0x10,// 4         
		0x27, 0x45, 0x45, 0x45, 0x39,// 5         
		0x3C, 0x4A, 0x49, 0x49, 0x30,// 6         
		0x01, 0x71, 0x09, 0x05, 0x03,// 7         
		0x36, 0x49, 0x49, 0x49, 0x36,// 8         
		0x06, 0x49, 0x49, 0x29, 0x1E,// 9         
		0x00, 0x36, 0x36, 0x00, 0x00,// :         
		0x00, 0x56, 0x36, 0x00, 0x00,// ;         
		0x00, 0x08, 0x14, 0x22, 0x41,// <         
		0x14, 0x14, 0x14, 0x14, 0x14,// =         
		0x41, 0x22, 0x14, 0x08, 0x00,// >         
		0x02, 0x01, 0x51, 0x09, 0x06,// ?         
		0x32, 0x49, 0x79, 0x41, 0x3E,// @         
		0x7E, 0x11, 0x11, 0x11, 0x7E,// A         
		0x7F, 0x49, 0x49, 0x49, 0x36,// B         
		0x3E, 0x41, 0x41, 0x41, 0x22,// C         
		0x7F, 0x41, 0x41, 0x22, 0x1C,// D         
		0x7F, 0x49, 0x49, 0x49, 0x41,// E         
		0x7F, 0x09, 0x09, 0x01, 0x01,// F         
		0x3E, 0x41, 0x41, 0x51, 0x32,// G         
		0x7F, 0x08, 0x08, 0x08, 0x7F,// H         
		0x00, 0x41, 0x7F, 0x41, 0x00,// I         
		0x20, 0x40, 0x41, 0x3F, 0x01,// J         
		0x7F, 0x08, 0x14, 0x22, 0x41,// K         
		0x7F, 0x40, 0x40, 0x40, 0x40,// L         
		0x7F, 0x02, 0x04, 0x02, 0x7F,// M         
		0x7F, 0x04, 0x08, 0x10, 0x7F,// N         
		0x3E, 0x41, 0x41, 0x41, 0x3E,// O         
		0x7F, 0x09, 0x09, 0x09, 0x06,// P         
		0x3E, 0x41, 0x51, 0x21, 0x5E,// Q         
		0x7F, 0x09, 0x19, 0x29, 0x46,// R         
		0x46, 0x49, 0x49, 0x49, 0x31,// S         
		0x01, 0x01, 0x7F, 0x01, 0x01,// T         
		0x3F, 0x40, 0x40, 0x40, 0x3F,// U         
		0x1F, 0x20, 0x40, 0x20, 0x1F,// V         
		0x7F, 0x20, 0x18, 0x20, 0x7F,// W         
		0x63, 0x14, 0x08, 0x14, 0x63,// X         
		0x03, 0x04, 0x78, 0x04, 0x03,// Y         
		0x61, 0x51, 0x49, 0x45, 0x43,// Z         
		0x00, 0x00, 0x7F, 0x41, 0x41,// [         
		0x02, 0x04, 0x08, 0x10, 0x20,// "\"       
		0x41, 0x41, 0x7F, 0x00, 0x00,// ]         
		0x04, 0x02, 0x01, 0x02, 0x04,// ^         
		0x40, 0x40, 0x40, 0x40, 0x40,// _         
		0x00, 0x01, 0x02, 0x04, 0x00,// `         
		0x20, 0x54, 0x54, 0x54, 0x78,// a         
		0x7F, 0x48, 0x44, 0x44, 0x38,// b         
		0x38, 0x44, 0x44, 0x44, 0x20,// c         
		0x38, 0x44, 0x44, 0x48, 0x7F,// d         
		0x38, 0x54, 0x54, 0x54, 0x18,// e         
		0x08, 0x7E, 0x09, 0x01, 0x02,// f         
		0x08, 0x14, 0x54, 0x54, 0x3C,// g         
		0x7F, 0x08, 0x04, 0x04, 0x78,// h         
		0x00, 0x44, 0x7D, 0x40, 0x00,// i         
		0x20, 0x40, 0x44, 0x3D, 0x00,// j         
		0x00, 0x7F, 0x10, 0x28, 0x44,// k         
		0x00, 0x41, 0x7F, 0x40, 0x00,// l         
		0x7C, 0x04, 0x18, 0x04, 0x78,// m         
		0x7C, 0x08, 0x04, 0x04, 0x78,// n         
		0x38, 0x44, 0x44, 0x44, 0x38,// o         
		0x7C, 0x14, 0x14, 0x14, 0x08,// p         
		0x08, 0x14, 0x14, 0x18, 0x7C,// q         
		0x7C, 0x08, 0x04, 0x04, 0x08,// r         
		0x48, 0x54, 0x54, 0x54, 0x20,// s         
		0x04, 0x3F, 0x44, 0x40, 0x20,// t         
		0x3C, 0x40, 0x40, 0x20, 0x7C,// u         
		0x1C, 0x20, 0x40, 0x20, 0x1C,// v         
		0x3C, 0x40, 0x30, 0x40, 0x3C,// w         
		0x44, 0x28, 0x10, 0x28, 0x44,// x         
		0x0C, 0x50, 0x50, 0x50, 0x3C,// y         
		0x44, 0x64, 0x54, 0x4C, 0x44,// z         
		0x00, 0x08, 0x36, 0x41, 0x00,// {         
		0x00, 0x00, 0x7F, 0x00, 0x00,// |         
		0x00, 0x41, 0x36, 0x08, 0x00,// }         
		0x08, 0x08, 0x2A, 0x1C, 0x08,// ->        
		0x08, 0x1C, 0x2A, 0x08, 0x08 // <-        

		};

