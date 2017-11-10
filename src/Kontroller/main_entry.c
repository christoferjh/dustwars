#include "lcd_engine.h"
#include <avr/io.h>
#include "connections.h"
#include "macros.h"
#include <util/delay.h>
#include "pong.h"
#include "buttons.h"
#include "touch.h"
#include <avr/interrupt.h> 
#include "game_missile.h"
#include "gui.h"
#include <stdlib.h>
#include "demo3d.h"
#include "images.h"
#include "pong.h"
#include "paint.h"
#include "system.h"
//Define functions
void flash_finish();

//System time
ISR(TIMER0_OVF_vect) {
	sys_time++;

	if (TOUCH_STATE!=TOUCH_STATE_OFF) { //Time for next touch reading
		start_reading_with_interrupt();
	}
}

//Create and launch a new meny
void gui_button_meny(LCD_engine *lcd, TouchCalibration *TC) {
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
	ButtonsMeny BMA;
	BMA.title = "Buttons";
	BMA.RootButton = 0;
	MenyButton b[3];
	b[0] = makeButton("Test", 1);
	b[2] = makeButton("Back", 3);
	addMenyButton(&BMA, &b[2]);
	addMenyButton(&BMA, &b[0]);
	int valde = 0;
	while (1) {
		valde = Buttons_meny(lcd, TC, &BMA, 3);
		if (valde == 1) {
			test_buttons(lcd);
		} else if (valde == 2) {
		} else {
			return;
		}
	}
}
//Create and launch a new meny
void gui_touch_meny(LCD_engine *lcd, TouchCalibration *TC) {
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
	ButtonsMeny BMA;
	BMA.title = "Touch";
	BMA.RootButton = 0;
	MenyButton b[3];
	b[0] = makeButton("Calibrate", 1);
	b[1] = makeButton("Test", 2);
	b[2] = makeButton("Back", 3);
	addMenyButton(&BMA, &b[2]);
	addMenyButton(&BMA, &b[1]);
	addMenyButton(&BMA, &b[0]);
	int valde = 0;
	while (1) {
		valde = Buttons_meny(lcd, TC, &BMA, 3);
		if (valde == 1) {
			calibrate_touch(lcd, TC);
		} else if (valde == 2) {
			test_touch(lcd);
		} else {
			return;
		}
	}
}
//Create and launch a new meny
void gui_screen_meny(LCD_engine *lcd, TouchCalibration *TC) {
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
	ButtonsMeny BMA;
	BMA.title = "Screen";
	BMA.RootButton = 0;
	MenyButton b[3];
	b[0] = makeButton("Reversed", 1);
	b[1] = makeButton("Test", 2);
	b[2] = makeButton("Back", 3);
	addMenyButton(&BMA, &b[2]);
	addMenyButton(&BMA, &b[0]);
	int valde = 0;
	while (1) {
		valde = Buttons_meny(lcd, TC, &BMA, 1);
		if (valde == 1) {
			if (lcd->DISPINVERTED) {
				DisplaySetDispInverted(lcd, 0);
				b[0].text = "Reversed";
			} else {
				DisplaySetDispInverted(lcd, 1);
				b[0].text = "Normal";
			}

		} else {
			return;
		}
	}
}
//Create and launch a new meny
void gui_system_meny(LCD_engine *lcd, TouchCalibration *TC) {
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
	ButtonsMeny BMA;
	BMA.title = "System";
	BMA.RootButton = 0;
	MenyButton b[4];
	b[0] = makeButton("Touch", 1);
	b[1] = makeButton("Buttons", 2);
	b[2] = makeButton("Screen", 3);
	b[3] = makeButton("Back", 4);
	addMenyButton(&BMA, &b[3]);
	addMenyButton(&BMA, &b[2]);
	addMenyButton(&BMA, &b[1]);
	addMenyButton(&BMA, &b[0]);
	int valde = 0;
	while (1) {
		valde = Buttons_meny(lcd, TC, &BMA, 2);
		if (valde == 1) {
			gui_touch_meny(lcd, TC);
		} else if (valde == 2) {
			gui_button_meny(lcd, TC);
		} else if (valde == 3) {
			gui_screen_meny(lcd, TC);
		} else {
			return;
		}
	}
}
//Create and launch a new meny
void gui_games_meny(LCD_engine *lcd, TouchCalibration *TC) {
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();

	ButtonsMeny BMA;
	BMA.title = "Games";
	BMA.RootButton = 0;
	MenyButton b[5];
	b[0] = makeButton("Pong", 1);
	b[1] = makeButton("Missiles", 2);
	b[2] = makeButton("Paint", 3);
	b[3] = makeButton("3d", 4);
	b[4] = makeButton("Back", 5);
	addMenyButton(&BMA, &b[4]);
	addMenyButton(&BMA, &b[3]);
	addMenyButton(&BMA, &b[2]);
	addMenyButton(&BMA, &b[1]);
	addMenyButton(&BMA, &b[0]);

	int valde = 0;

	while (1) {

		valde = Buttons_meny(lcd, TC, &BMA, 4);
		if (valde == 1) {
			game_ball(lcd, TC);
		} else if (valde == 2) {
			game_missile(lcd, TC);
		} else if (valde == 3) {
			paint(lcd, TC);
		} else if (valde == 4) {
			start_3dDemo(lcd, TC);
		} else {
			return;
		}

	}
}

//About this prototype
void about(LCD_engine *lcd) {
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
	LCD_setPage(0);
	rprintfStrCenter("This is the work of");
	LCD_setPage(1);
	rprintfStrCenter("Christofer");
	LCD_setPage(2);
	rprintfStrCenter("Johansson Hiitti");
	LCD_setPage(3);
	rprintfStrCenter("T08");

	LCD_setPage(7);
	rprintfStrCenter("FIM project 2011");

	while (1) {
		if (START_isPressedAndRel()) {
			drawStartImage(lcd); //Show a nice image
			break;
		}
	}
}

//Root meny
void main_gui_meny(LCD_engine *lcd, TouchCalibration *TC) {
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
	LCD_setPage(0);
	LCD_setColumn(0);
	rprintfStr("meny");

	ButtonsMeny BMA;
	BMA.title = "Main meny";
	BMA.RootButton = 0;
	MenyButton b[3];
	b[0] = makeButton("Games", 1);
	b[1] = makeButton("System", 2);
	b[2] = makeButton("About", 3);
	addMenyButton(&BMA, &b[2]);
	addMenyButton(&BMA, &b[1]);
	addMenyButton(&BMA, &b[0]);

	int valde = 0;

	while (1) {

		valde = Buttons_meny(lcd, TC, &BMA, 4);
		if (valde == 1) {
			gui_games_meny(lcd, TC);
		} else if (valde == 2) {
			gui_system_meny(lcd, TC);
		} else if (valde == 3) {
			about(lcd);
		}

	}

}

//Main entry point
int main(void) {
	flash_finish(); //Fun to have, but dont do anything anymore, still like to have it here
	LCD_engine *lcd = init_lcd(); //Get a LCD buffer and init LCD
	button_init(); //Init buttons
	//The calibration
	TouchCalibration TC;
	//Init the sys_time
	TCCR0B = 0b00000011; //64 prescaler
	TIMSK0 = 0x01;
	sei(); //Init interrupts


	touch_init_advance(&TC);//Init the interrupt handeled touch

	unsigned char starts = eeprom_read(10);
	if (starts == 255) {//just flashed...
		set_calibration(&TC, 37, 164, 47, 86); //Sets a pretty good calibration
		calibrate_touch(lcd, &TC);

	}
	if (starts != 254) {
		eeprom_write(10, starts + 1);
	}

	//If you would like to know how many times it have been started..
	//LCD_setPage(3);
	//LCD_setColumn(0);
	//rprintfStr("eeprom:");
	//rprintfInt(starts);

	//Draw the nice start image
	drawStartImage(lcd);
	//Main meny, never exits from here
	main_gui_meny(lcd, &TC);

	//Just in case...
	while (1) {
	}

}

//Maybe set some LEDs on?
void flash_finish() {

}

