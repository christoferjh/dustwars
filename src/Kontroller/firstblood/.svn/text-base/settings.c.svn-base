#include <avr/io.h>
#include "connections.h"
#include "macros.h"
#include "lcd_engine.h"
#include "touch.h"
#include <util/delay.h>
#include "buttons.h"
#include "gui.h"
#include "system.h"
#include "dustwarsIDs.h"

#include "killer.h"


//Check if user would like to quit game
int in_meny(LCD_engine *lcd, TouchCalibration *TC) {//return 1 if exit game
//This is really cool, it starts a new lcd buffer so the old one is intact. To use for ingame menys etc, for example a pause meny.
	DOGENABLE;
	LCD_setPage(0);
	LCD_setColumn(0);
	rprintfStr("Pressed START");
	LCD_engine * newLCD = get_new_ram(); //The cool stuff, allocates a new buffer
	newLCD->DISPINVERTED = lcd->DISPINVERTED;
	setDubbelbuffer(newLCD, 1);
	LCD_RAM_CLR(newLCD);
	pushRAM(newLCD);

	ButtonsMeny BMA;
	BMA.title = "Paused";
	BMA.RootButton = 0;
	MenyButton b[3];
	b[0] = makeButton("QUIT", 1);
	b[1] = makeButton("Settings", 2);
	b[2] = makeButton("Return", 0);
	addMenyButton(&BMA, &b[2]);
	addMenyButton(&BMA, &b[1]);
	addMenyButton(&BMA, &b[0]);

	//Stoppar roboten
	sendZigByte(ID_DRIVE,0x00);

	int valde = 2;
	while (valde==2){
	valde = Buttons_meny(newLCD, TC, &BMA, 3);
	if (valde==2){
		gui_screen_meny(newLCD,TC);
	}
	}

	trash_ram(newLCD); //Trashes the new buffer since we are done with it
	LCD_CLR();
	DOGENABLE;
	redrawRAM(lcd); //Then just like nothing have happend, the game countinues, the old buffer is redrawn.
	if (valde<0){//Pressed start
		valde=0;
		}
	return valde;
}
