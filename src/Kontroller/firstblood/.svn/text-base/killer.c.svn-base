#include <avr/io.h>
#include "connections.h"
#include "macros.h"
#include "lcd_engine.h"
#include "touch.h"
#include <util/delay.h>
#include "buttons.h"
#include "gui.h"
#include "system.h"
#include "zigbeeny.h"
#include "dustwarsIDs.h"
#include "killer.h"
#include "images.h"
#include "ani.h"


int start_killer(LCD_engine *lcd_old ,TouchCalibration *TC){
	DOGENABLE;
	LCD_setPage(0);
	LCD_setColumn(0);
	rprintfStr("Pressed START");
	LCD_engine * lcd = get_new_ram(); //The cool stuff, allocates a new buffer
	setDubbelbuffer(lcd, 1);
	LCD_RAM_CLR(lcd);
	
		//Start message on leds
	sendZigByte(ID_LEDS,ANI_FLASH);
	//LCD_drawImageToRam(lcd,Darth_im);
	LCD_drawImageToRam(lcd,Darth_im);
	redrawRAM(lcd);
	sendZigByte(ID_DRIVE,(2)<<4 | (1));
	char time=0;
	char eye=0;
	char dir=1;
	while (1){
	LCD_setPage(0);
	LCD_setColumn(0);
	rprintfStr("KILLER MODE!");	
	pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
		
		printfStr(lcd,"*",eye,10);
	time++;
	if (time>2){
		if (dir){
			eye++;
		}else {
			eye--;
		}
		if (eye>60){
		//eye=0;
			dir=0;
			}
		if (eye<=0){
			//eye=0;
			dir=1;
		}

	time=0;
		}
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	printfStr(lcd,"|",eye,10);
	
	pushRAM(lcd);	
	}	



	trash_ram(lcd); //Trashes the new buffer since we are done with it
	LCD_CLR();
	DOGENABLE;
	redrawRAM(lcd_old); //Then just like nothing have happend, the game countinues, the old buffer is redrawn.




}
